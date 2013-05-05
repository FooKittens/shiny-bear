// Renders normals into one render target, and depth into a second target.

cbuffer perFrame
{
  float g_zfar;
}

cbuffer perObject
{
  float4x4 g_proj;
  float4x4 g_view;
  float4x4 g_world;
  float4x4 g_WVP;
};

struct GeometryVSIn
{
  float3 position : POSITION0;
  float3 normal : NORMAL0;
  float4 diffuse : COLOR0;
  float4 specular : COLOR1;
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

// Helper methods for packing normal values.
float PackRange(float f)
{
  return (f * 0.5f) + 0.5f;
}

float UnPackRange(float f)
{
  return (f - 0.5f) * 2.0f;
}

// Used for send normal and specular data to pixelshader.
GeometryVSOut VSNormalSpecularExp(GeometryVSIn input)
{
  GeometryVSOut output;
  output.position = mul(float4(input.position, 1.0f), g_WVP);
  output.normal = mul(float4(input.normal, 0.0f), g_world);
  
  output.diffuse = input.diffuse;
  output.specular = input.specular;

  output.depth = (output.position.z * output.position.w) / g_zfar;
  return output;
}

// Writes normaldata into the rgb components and the specular exponent into the alpha channel.
PSNormalMRTOUT PSNormalSpecularExp(GeometryVSOut input)
{
  PSNormalMRTOUT output;
  output.rt0 = float4(PackRange(input.normal.x),
   PackRange(input.normal.y), PackRange(input.normal.z), input.specular.a);
  output.rt1 = float4(input.depth, input.depth, input.depth, input.depth);
  return output;
}

// Used to render normals and specular exponents into a render target.
technique NormalTech
{
  pass pass0
  {
    vertexShader = compile vs_3_0 VSNormalSpecularExp();
    pixelShader = compile ps_3_0 PSNormalSpecularExp();
  
    AlphaBlendEnable = true;
  }

  
}