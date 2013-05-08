
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
  float4x4 g_invView;
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
  float3 a;
};

Light g_light;

texture g_normalMap;
sampler g_normalSampler = sampler_state
{
  Texture = <g_normalMap>;
  MinFilter = POINT;
  MagFilter = POINT;
  MipFilter = POINT;

};

texture g_depthMap;
sampler g_depthSampler = sampler_state
{
  Texture = <g_depthMap>;
  MinFilter = POINT;
  MagFilter = POINT;
  MipFilter = POINT;
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


float3 UnProjectPosition(float2 uv)
{
  float z = tex2D(g_depthSampler, uv).r;
  float x = uv.x * 2 - 1;
  float y = (1 - uv.y)  * 2 -1;

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
  //float2 texCoords : TEXCOORD0;

  // Used to store the light direction transformed into view space.
  float3 lightDir : TEXCOORD1;
};

VSDiffuseOut VSDiffuse(VSDiffuseIn input)
{
  VSDiffuseOut o;
  o.position = mul(float4(input.position, 1.0f), g_WVP);
  o.screenPos = o.position;
  o.lightDir = normalize(mul(float4(g_light.direction, 0), g_invView).xyz);

  //o.texCoords = o.position.xy * 0.5f + 0.5f;
  //o.texCoords.y = 1.0f - o.texCoords.y;
  //o.texCoords += g_halfPixel;

  return o;
}

MRTOUT PSDiffuse(VSDiffuseOut input)
{
  MRTOUT mOut = (MRTOUT)0;
  float2 uv = input.screenPos.xy * 0.5f + 0.5f;
  uv.y = 1.0f - uv.y;
  uv += g_halfPixel;


  float4 val = tex2D(g_normalSampler, uv);

  if(val.a <= 0)
  {
    return mOut;
  }

  // Retrieve normal and bring into correct range.
  float3 normal = normalize(2.0f * val.xyz - 1.0f);

  // Bring specular exponent range into values 0-255.
  float specularExp = max(val.a * 255, 1.0f);

  // Retrieve view-space position from texture coordinates. ( samples depthMap )
  float3 position = UnProjectPosition(uv);

  // Calculate diffuse light in view space.
  float diffuseIntensity = max(0.0f, dot(normal, normalize(-input.lightDir)));
  
  float specularIntensity = 0;
  if(diffuseIntensity > 0)
  {
    float3 viewVec = normalize(-position);
    float3 r = reflect(input.lightDir, normal);
    specularIntensity = pow(max(0.0f, dot(viewVec, r)), specularExp);
  }
  
  mOut.rt0 = g_light.color * diffuseIntensity;
  mOut.rt1 = g_light.color * specularIntensity;
  
  //mOut.rt0 = float4(position, 1.0f);

  return mOut; 
}

technique DiffuseLightTech
{
  pass p0
  {
    vertexshader = compile vs_3_0 VSDiffuse();
    pixelshader = compile ps_3_0 PSDiffuse();
    
    CullMode = None;
    AlphaBlendEnable = true;
    SrcBlend = One;
    DestBlend = One;
    BlendOp = ADD;
  }
}


float4 VSAmbient(float3 pos : POSITION0) : SV_POSITION
{
  return mul(float4(pos, 1.0f), g_WVP);
}

float4 PSAmbient(float4 pos : SV_POSITION) : SV_TARGET
{
  return g_light.color;
}

technique AmbientLightTech
{
  pass p0
  {
    vertexshader = compile vs_3_0 VSAmbient();
    pixelshader = compile ps_3_0 PSAmbient();
    
    CULLMODE = NONE;
    AlphaBlendEnable = true;
    SrcBlend = One;
    DestBlend = One;
    BlendOp = ADD;
  }
}


struct VSPointIn
{
  float3 position : POSITION0;
};

struct VSPointOut
{
  float4 position : SV_POSITION;
  float3 screenPos : TEXCOORD0;
  float3 lightPos : TEXCOORD1;
};


VSPointOut VSPoint(VSPointIn input)
{
  VSPointOut pOut;
  pOut.position = mul(float4(input.position, 1.0f), g_WVP);

  pOut.screenPos.xy = pOut.position.xy;
  pOut.screenPos.z = pOut.position.w;

  pOut.lightPos = mul(float4(g_light.position, 1.0f), g_view).xyz;

  return pOut;
}

MRTOUT PSPoint(VSPointOut input)
{
  MRTOUT mOut = (MRTOUT)0;

  float2 uv = (input.screenPos.xy / input.screenPos.z) * 0.5f + 0.5f;
  uv.y = 1.0f - uv.y;
  uv += g_halfPixel;

  float4 val = tex2D(g_normalSampler, uv);

  if(val.a <= 0)
  {
    //return mOut;
  }

  // Retrieve normal and bring into correct range.
  float3 normal = normalize(2.0f * val.xyz - 1.0f);

  // Bring specular exponent range into values 0-255.
  float specularExp = max(val.a * 255, 1.0f);

  // Retrieve view-space position from texture coordinates. ( samples depthMap )
  float3 position = UnProjectPosition(uv);

  float3 lightDir = (input.lightPos - position) / length(input.lightPos - position);
  float lightDist = distance(input.lightPos, position);

  lightDir = normalize(lightDir);

  float diffuseIntensity = max(0.0f, dot(normalize(lightDir), normal));

  if(diffuseIntensity > 0)
  {
    float att = 1.0f / (g_light.a.r + g_light.a.g * lightDist + g_light.a.b * pow(lightDist, 2));

    mOut.rt0 = diffuseIntensity * att * g_light.color;
    //mOut.rt0 = float4(1, 0, 0, 1);

    float3 viewVec = normalize(-position);
    float3 r = reflect(-lightDir, normal);
    float specularIntensity = pow(max(0.0f, dot(viewVec, r)), specularExp);
    if(specularIntensity > 0)
    {
      mOut.rt1 = specularIntensity * g_light.color * att;
    }
  }

  //mOut.rt0 = float4(position, 1);

  return mOut;
}

technique PointLightTech
{
  pass p0
  {
    vertexshader = compile vs_3_0 VSPoint();
    pixelshader = compile ps_3_0 PSPoint();
 
    CullMode = NONE;
    AlphaBlendEnable = true;
    SrcBlend = One;
    DestBlend = One;
    BlendOp = ADD;
    //FillMode = WIREFRAME;
  }

}




