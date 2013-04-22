

uniform extern float4x4 gWVP;
texture gText;

sampler t2samp = sampler_state
{
  Texture = gText;
  MinFilter = Linear;
  MagFilter = Linear;
};

struct VSInput
{
  float3 pos : POSITION0;
  float2 texCoords : TEXCOORD0;
};

struct VSOutput
{
  float4 pos : POSITION0;
  float2 texCoords : TEXCOORD0;
};

VSOutput VS(VSInput input)
{

  VSOutput ov = (VSOutput)0;

  ov.pos = mul(float4(input.pos, 1.0f), gWVP);
  ov.texCoords = input.texCoords;
  return ov;
}

float4 PS(VSOutput input) : COLOR
{
  return tex2D(t2samp, input.texCoords);
}

technique SimpleTechnique
{

  pass P0
  {

    vertexShader = compile vs_2_0 VS();
    pixelShader = compile ps_2_0 PS();

    //FillMode = Wireframe;
  }
}