// Derp shader

cbuffer b
{
  float4x4 g_world;
  float4x4 g_view;
  float4x4 g_projection;
};

struct VS_INPUT
{
  float3 pos : POSITION0;
  float3 norm : NORMAL0;
  float4 diffuse : COLOR0;
  float4 specular : COLOR1;
};

struct PS_INPUT
{
  float4 pos : SV_POSITION;
  float3 norm : TEXCOORD0;
  float4 diffuse : COLOR0;
  float4 specular : COLOR1;
};

PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT ps;
  ps.pos = mul(float4(input.pos, 1.0f), g_world);
  ps.pos = mul(ps.pos, g_view);
  ps.pos = mul(ps.pos, g_projection);
  ps.norm = input.norm;
  ps.diffuse = input.diffuse;
  ps.specular = input.specular;
  return ps;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
  return float4(1.0f, 1.0f, 1.0f, 1.0f);
}


technique BaseTech
{
  pass p0
  {
    vertexShader = compile vs_2_0 VS();
    pixelShader = compile ps_2_0 PS();
  }
}