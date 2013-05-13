
///////////////////////////////////////
////// COMBINE PASS ///////////////////
///////////////////////////////////////

cbuffer perFrame
{
  texture g_lightMap;
  float2 g_halfPixel;
};

cbuffer perObject
{
  float4x4 g_WVP;
};

sampler g_lightSampler = sampler_state
{
  Texture = <g_lightMap>;
  MinFilter = ANISOTROPIC;
  MagFilter = LINEAR;
  MipFilter = LINEAR;
  MaxAnisotropy = 8;
};

texture g_materialMap;
sampler g_materialSampler = sampler_state
{
  Texture = <g_materialMap>;
  MinFilter = POINT;
  MagFilter = POINT;
  MipFilter = POINT;
};


struct VSCombineInput
{
  float4 position : POSITION0;
};

struct VSCombineOutput
{
  float4 position : SV_POSITION;
  float4 pp : TEXCOORD0;

};

VSCombineOutput VSCombine(VSCombineInput input)
{
  VSCombineOutput output;
  output.position = input.position;
  output.pp = output.position;
  return output;
}

float4 PSCombine(VSCombineOutput input) : SV_TARGET
{
  float2 uv = input.pp.xy / input.pp.w;

  uv *= 0.5f;
  uv += 0.5f;
  uv.y *= -1.0f;
  uv.xy = uv.xy + float2(0.5f / 1280.0f, 0.5f / 720.0f);

  float4 mat = tex2D(g_materialSampler, uv);

  float4 col = tex2D(g_lightSampler, uv);

  return saturate(float4(mat.rgb * col.rgb, 1.0f) ) +
         saturate(float4(mat.rgb * col.rgb * col.a, 1.0f));
}

technique CombineTech
{
  pass p0
  {
    vertexshader = compile vs_3_0 VSCombine();
    pixelshader = compile ps_3_0 PSCombine();
    //FillMode = WIREFRAME;
  }
}

///////////////////////////////////////
////// RENDERSCREEN PASS //////////////
///////////////////////////////////////


struct VSRTSOUT
{
  float4 pos : SV_POSITION;
  float4 pp : TEXCOORD0;
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
VSRTSOUT VSRenderToScreen(float4 pos : POSITION0)
{
  VSRTSOUT output;
  output.pos = pos;
  output.pp = pos;

  float2 uv = pos.xy / pos.w;

  uv *= 0.5f;
  uv += 0.5f;
  uv.y *= -1.0f;
  uv.xy = uv.xy + float2(0.5f / 1280.0f, 0.5f / 720.0f);

  output.pp = pos * pos.w;
  return output;
}

float4 PSRenderToScreen(float4 pos : POSITION0, float4 tex : TEXCOORD0) : SV_TARGET
{
  float2 uv = tex.xy;

  uv *= 0.5f;
  uv += 0.5f;
  uv.y = 1.0f - uv.y;
  uv.xy = uv.xy + float2(0.5f / 1280.0f, 0.5f / 720.0f);

  //return float4(.95f, .25f, .25f, 1.0f);
  return tex2D(samplerstate, uv);
}

// Technique used to render a fullscreen quad to with a texture to the screen.
technique RenderToScreen
{
  pass pass0
  {
    vertexshader = compile vs_3_0 VSRenderToScreen();
    pixelshader = compile ps_3_0 PSRenderToScreen();
  }
}