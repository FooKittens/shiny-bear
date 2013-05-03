
#define LIGHT_TYPE_AMBIENT 1
#define LIGHT_TYPE_DIFFUSE 2
#define LIGHT_TYPE_POINT   3
#define LIGHT_TYPE_SPOT    4

// Updated for each geometry object.
cbuffer perObject
{
  float4x4 g_world;
};


// Updated once per frame.
cbuffer perFrame
{
  float4x4 g_view;
  float4x4 g_projection;
  float g_znear;
  float g_zfar;

  // Used for specular calculations.
  float3 g_cameraPosition;
};

// Helper methods for packing normal values.
float PackRange(float f)
{
  return (f * 0.5f) + 0.5f;
}

float UnPackRange(float f)
{
  return (f - 0.5f) * 2.0f;
}


///////////////////////////////////////
////// RENDERSCREEN PASS //////////////
///////////////////////////////////////

struct TextureVSIn
{

  float4 position : POSITION0;
  float2 texcoords : TEXCOORD0;
};

struct TextureVSOut
{
  float4 position : SV_POSITION;
  float2 texcoords : TEXCOORD0;
};


// Used for fullscreen texture rendering.
texture g_texture;

// Sampler used to sample the texture used for screen rendering.
sampler samplerstate = sampler_state
{
  Texture = <g_texture>;
  MinFilter = LINEAR;
  MagFilter = LINEAR;
  MipFilter = LINEAR;
  AddressU = CLAMP;
  AddressV = CLAMP;
};

// Expects coordinates to be in screen space.
TextureVSOut VSRenderToScreen(TextureVSIn input)
{
  TextureVSOut output;
  output.position = input.position;
  output.texcoords = input.texcoords;
  return output;
}

float4 PSRenderToScreen(TextureVSOut input) : SV_TARGET
{
  //return float4(.95f, .25f, .25f, 1.0f);
  return tex2D(samplerstate, input.texcoords);
}

// Technique used to render a fullscreen quad to with a texture to the screen.
technique RenderToScreen
{
  pass pass0
  {
    vertexshader = compile vs_2_0 VSRenderToScreen();
    pixelshader = compile ps_2_0 PSRenderToScreen();
  }
}

///////////////////////////////////////
////// NORMAL PASS ////////////////////
///////////////////////////////////////

struct GeometryVSIn
{
  float3 position : POSITION0;
  float3 normal : NORMAL0;
  float4 diffuse : COLOR0;
  float4 specular : COLOR0;
};

struct GeometryVSOut
{
  float4 position : SV_POSITION;
  float4 normal : TEXCOORD0;
  float4 diffuse : COLOR0;
  float4 specular : COLOR1;
  float  depth : TEXCOORD1;
};

struct PSNormalMRTOUT
{
  float4 rt0 : COLOR0;
  float4 rt1 : COLOR1;
};

// Used for send normal and specular data to pixelshader.
GeometryVSOut VSNormalSpecularExp(GeometryVSIn input)
{
  GeometryVSOut output;
  output.position = mul(float4(input.position, 1.0f), g_world);
  output.position = mul(output.position, g_view);
  output.position = mul(output.position, g_projection);

  output.normal = mul(float4(input.normal, 0.0f), g_world);
  //output.normal = mul(float4(input.normal.xyz, 0.0f), g_view);
  output.diffuse = input.diffuse;
  output.specular = input.specular;

  output.depth = (output.position.z * output.position.w) / g_zfar;
  return output;
}

// Writes normaldata into the rgb components and the specular exponent into the alpha channel.
PSNormalMRTOUT PSNormalSpecularExp(GeometryVSOut input)
{
  PSNormalMRTOUT output;
  output.rt0 = float4(PackRange(input.normal.x), PackRange(input.normal.y), PackRange(input.normal.z), input.specular.a);
  //output.rt0 = float4(abs(input.normal.x), abs(input.normal.y), abs(input.normal.z), input.specular.a);
  output.rt1 = float4(input.depth, input.depth, input.depth, input.depth);
  return output;
  //return 
  //return float4(input.normal.xyz, input.specular.a);
}

// Used to render normals and specular exponents into a render target.
technique NormalTech
{
  pass pass0
  {
    vertexShader = compile vs_2_0 VSNormalSpecularExp();
    pixelShader = compile ps_2_0 PSNormalSpecularExp();
  }
}

///////////////////////////////////////
////// LIGHT PASS /////////////////////
///////////////////////////////////////

struct LightVSIn
{
  float4 position : POSITION0;
  float2 texcoords : TEXCOORD0;
};

struct LightVSOut
{
  float4 position : SV_POSITION;
  float2 texcoords : TEXCOORD0;
};

struct MRTOUT
{
  float4 rt0 : COLOR0;
  float4 rt1 : COLOR1;
};

struct Light
{
  float3 position;
  uint type;
  float3 direction;
  float4 color;
};

Light g_light;

texture g_normalMap;
sampler g_normalSampler = sampler_state
{
  Texture = <g_normalMap>;
  MinFilter = POINT;
  MagFilter = POINT;
  MipFilter = POINT;
  AddressU = CLAMP;
  AddressV = CLAMP;
};


LightVSOut VSLightPassMRT(LightVSIn input)
{
  LightVSOut output;
  output.position = input.position;
  output.texcoords = input.texcoords; // HARDCODED HALF PIXEL SIZE...
  return output;
}

struct LightColor
{
  float4 diffuse;
  float4 specular;
};

MRTOUT DiffuseLight(float3 normal, float exp)
{
  MRTOUT col;
  float inten = max(0, dot(normalize(normal), normalize(-g_light.direction)));
  col.rt0 = inten * g_light.color;
  col.rt1 = float4(0, 0, 0, 0);
  return col;
}

MRTOUT AmbientLight()
{
  MRTOUT col;
  col.rt0 = float4(1, 0, 1, 0);;
  col.rt1 = (float4)0;
  return col;
}

MRTOUT PointLight()
{
  MRTOUT col;
  col.rt0 = float4(1, 0, 1, 0);
  col.rt1 = (float4)0;
  return col;
}

MRTOUT SpotLight()
{
  MRTOUT col;
  col.rt0 = float4(1, 1, 1, 1);
  col.rt1 = (float4)0;
  return col;
}

MRTOUT PSLightPassMRT(LightVSOut input)
{
  float4 nCol = tex2D(g_normalSampler, input.texcoords);
  float3 normal = float3(UnPackRange(nCol.x), UnPackRange(nCol.y), UnPackRange(nCol.z));


  switch(g_light.type)
  {
    case LIGHT_TYPE_AMBIENT: return AmbientLight();
    case LIGHT_TYPE_DIFFUSE: return DiffuseLight(normal, nCol.a * 255);
    case LIGHT_TYPE_POINT:   return PointLight();
    case LIGHT_TYPE_SPOT:    return SpotLight();
  }

  MRTOUT output;
  output.rt0 = float4(1, 1, 1, 1);
  output.rt1 = float4(1, 1, 1, 1);
  return output;
}


technique LightPassMRT
{
  pass pass0
  {
    vertexshader = compile vs_2_0 VSLightPassMRT();
    pixelshader = compile ps_2_0 PSLightPassMRT();

    AlphaBlendEnable = true;
    SrcBlend = One;
    DestBlend = One;
    BlendOp = ADD;
  }
}



///////////////////////////////////////
////// COMBINE PASS ///////////////////
///////////////////////////////////////

texture g_diffuseMap;
texture g_specularMap;

sampler g_diffSampler = sampler_state
{
  Texture = <g_diffuseMap>;
  MinFilter = ANISOTROPIC;
  MagFilter = LINEAR;
  MipFilter = LINEAR;
  MaxAnisotropy = 16;
};

sampler g_specSampler = sampler_state
{
  Texture = <g_specularMap>;
  MinFilter = LINEAR;
  MagFilter = LINEAR;
  MipFilter = LINEAR;
};


struct VSCombineInput
{
  float3 position : POSITION0;
  float3 normal : NORMAL0;
  float4 diffuse : COLOR0;
  float4 specular : COLOR1;
};

struct VSCombineOutput
{
  float4 position : SV_POSITION;
  float4 diffuse : COLOR0;
  float4 specular : COLOR1;
  float4 pp : TEXCOORD0;
};

VSCombineOutput VSCombine(VSCombineInput input)
{
  VSCombineOutput output;
  output.position = mul(float4(input.position, 1.0f), g_world);
  output.position = mul(output.position, mul(g_view, g_projection));
  output.diffuse = input.diffuse;
  output.specular = input.specular;
  output.pp = (output.position);
  //output.texcoords = output.position.xy / output.position.w;
  //output.texcoords.x += 0.5f;
  //output.texcoords.y += 0.5f;
  return output;
}

float4 PSCombine(VSCombineOutput input) : SV_TARGET
{
  float2 uv = input.pp.xy / input.pp.w;

  uv *= 0.5f;
  uv += 0.5f;
  uv.y *= -1.0f;
  uv.xy = uv.xy; // + float2(1.5f / 1920.0f, 1.5f / 1080.0f);


  return input.diffuse * float4(0.15f, 0.15f, 0.15f, 1.0f) + input.diffuse * tex2D(g_diffSampler, uv);
}

technique CombineTech
{
  pass p0
  {
    vertexshader = compile vs_2_0 VSCombine();
    pixelshader = compile ps_2_0 PSCombine();
  }
}