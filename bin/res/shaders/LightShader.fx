
#define LIGHT_TYPE_AMBIENT 1
#define LIGHT_TYPE_DIFFUSE 2
#define LIGHT_TYPE_POINT   3
#define LIGHT_TYPE_SPOT    4

// Updated for each geometry object.
cbuffer perObject
{
  float4x4 g_world;
  float4x4 g_WVP;
};


// Updated once per frame.
cbuffer perFrame
{
  float4x4 g_view;
  float4x4 g_projection;
  float g_znear;
  float g_zfar;

  float2 g_halfPixel;

  // Used for specular calculations.
  float3 g_cameraPosition;
  float4x4 g_invProjection;
};



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

texture g_depthMap;
sampler g_depthSampler = sampler_state
{
  Texture = <g_depthMap>;
  MinFilter = POINT;
  MagFilter = POINT;
  MipFilter = POINT;
  AddressU = CLAMP;
  AddressV = CLAMP;
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


LightVSOut VSLightPassMRT(LightVSIn input)
{
  LightVSOut output;
  output.position = mul(input.position, g_WVP);
  output.texcoords = input.texcoords;
  return output; 
}


MRTOUT DiffuseLight(float3 normal, float sexp)
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
  float4 nCol = tex2D(g_normalSampler, input.texcoords + float2(0.5f / 1280.0f, 0.5f / 720.0f));
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


float3 UnProjectPosition(float2 uv)
{
  float z = UnPackRange(tex2D(g_depthSampler, uv).r);
  float x = uv.x; // * 2 - 1;
  float y = (1 - uv.y); // * 2 -1;

  // Un-Projected position.
  float4 upPos = mul(float4(x, y, z, 1.0f), g_invProjection);
  return upPos.xyz / upPos.w;
}

struct VSDiffuseIn
{
  float3 position : POSITION0;
};

struct VSDiffuseOut
{
  float4 position : SV_POSITION;
  float4 screenPos : TEXCOORD0;
};

VSDiffuseOut VSDiffuse(VSDiffuseIn input)
{
  VSDiffuseOut o;
  o.position = mul(float4(input.position, 1.0f), g_WVP);
  o.screenPos = o.position;
  return o;
}

MRTOUT PSDiffuse(VSDiffuseOut input)
{
  MRTOUT mOut;
  float2 uv = input.screenPos.xy * 0.5f + 0.5f;
  uv.y = 1.0f - uv.y;
  uv += g_halfPixel;


  float4 val = tex2D(g_normalSampler, uv);
  float3 normal = normalize(float3(UnPackRange(val.x), UnPackRange(val.y), UnPackRange(val.z)));
  float specularExp = val.a * 255;
  if(val.a == 0)
  {
    mOut.rt0 = float4(0, 0, 0, 0);
    mOut.rt1 = float4(0, 0, 0, 0);
    return mOut;
  }


  float3 position = UnProjectPosition(input.screenPos.xy );



  
  
  // Diffuse calculation.
  float inten = max(0, dot(normal, normalize(-g_light.direction)));
  mOut.rt0 = inten * g_light.color;
  
  // Specular.
  //float sInten = max
  if(inten > 0)
  {

    float3 viewVec = normalize(g_cameraPosition - position);
    float3 r = reflect(normalize(g_light.direction), normal);
    mOut.rt1 = pow(max(dot(viewVec, r), 0), specularExp) * g_light.color;
    //mOut.rt1 = float4(specularExp / 255.0f, 0, 0, 1);
    //mOut.rt1 = float4(position, 1);
    return mOut;
  }  

  mOut.rt0 = float4(0, 0, 0, 1);  
  //mOut.rt1 = float4(1, 0, 0, 1);
  return mOut; 
}

technique DiffuseLightTech
{
  pass p0
  {
    vertexshader = compile vs_3_0 VSDiffuse();
    pixelshader = compile ps_3_0 PSDiffuse();
    
    //CullMode = None;
    AlphaBlendEnable = true;
    SrcBlend = One;
    DestBlend = One;
    BlendOp = ADD;
  }
}


technique LightMRTTech
{
  pass pass0
  {
    vertexshader = compile vs_3_0 VSLightPassMRT();
    pixelshader = compile ps_3_0 PSLightPassMRT();

    AlphaBlendEnable = true;
    SrcBlend = One;
    DestBlend = One;
    BlendOp = ADD;
  }
}


