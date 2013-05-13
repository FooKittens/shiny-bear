// Renders normals into one render target, and depth into a second target.

cbuffer perFrame
{
  float g_zfar;
  float4x4 g_view;
}

cbuffer perObject
{
  float4x4 g_world;
  float4x4 g_WVP;
  float4x4 g_inverseTranspose;
};

struct GeometryVSIn
{
  float3 position : POSITION0;
  float3 normal : NORMAL0;
  float4 color : COLOR0;
};

struct GeometryVSOut
{
  float4 position : SV_POSITION;
  float3 normal : TEXCOORD0;
  float4 color : COLOR0;
  float2 depth : TEXCOORD1;
};

struct MRTOUT
{
  float4 rt0 : COLOR0;
  float4 rt1 : COLOR1;
  float4 rt2 : COLOR2;
};

// Used for send normal and specular data to pixelshader.
GeometryVSOut VSNormalSpecularExp(GeometryVSIn input)
{
  GeometryVSOut output;

  // Transform position into screen space.
  output.position = mul(float4(input.position, 1.0f), g_WVP);
  
  // Transform the normal into view space.
  // Uses the inverse transpose of the world-view matrix.
  //output.normal = normalize(mul(float4(input.normal, 0.0f), g_inverseTranspose).xyz);
  output.normal = float4(input.normal, 0);

  //output.normal = normalize(mul(float4(input.normal, 0.0f), mul(g_world,  g_view)));

  // Pass on material values.
  output.color = input.color;

  float z = mul(float4(input.position, 1), mul(g_world, g_view)).z;
  output.depth.x = output.position.z;
  output.depth.y = output.position.w;
  return output;
}

// Writes normaldata into the rgb components and the specular exponent into the alpha channel.
MRTOUT PSNormalSpecularExp(GeometryVSOut input)
{
  float3 normal = normalize(mul(input.normal, g_inverseTranspose).xyz);

  MRTOUT output;
  output.rt0 = float4(0.5f * (normalize(normal) + 1.0f), input.color.a);
  output.rt1 = input.depth.x / input.depth.y;
  output.rt2 = float4(input.color.rgb, 1.0f);
  return output;
}

// Used to render normals and specular exponents into a render target.
technique NormalTech
{
  pass pass0
  {
    vertexShader = compile vs_3_0 VSNormalSpecularExp();
    pixelShader = compile ps_3_0 PSNormalSpecularExp();
  }

  
}

float4 VSClearGBuffer(float4 pos : POSITION0) : SV_POSITION
{
  return pos;
}

MRTOUT PSClearGBuffer(float4 pos : POSITION0) 
{
  // Clear normal buffer to "zero"
  MRTOUT mOut = (MRTOUT)0;

  // Results in a zero normal with 0 in specular.
  mOut.rt0 = float4(0.5f, 0.5f, 0.5f, 0.0f);

  mOut.rt1 = float4(0, 0, 0, 0);
  mOut.rt2 = float4(0, 0, 0, 1.0f);
  return mOut;
}

technique ClearGBufferTech
{
  pass p0
  {
    vertexShader = compile vs_3_0 VSClearGBuffer();
    pixelShader = compile ps_3_0 PSClearGBuffer();
  }

}




