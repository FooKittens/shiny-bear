
///////////////////////////////////////
////// COMBINE PASS ///////////////////
///////////////////////////////////////

cbuffer perFrame
{
  texture g_diffuseMap;
  texture g_specularMap;  
};

cbuffer perObject
{
  float4x4 g_WVP;
};

sampler g_diffSampler = sampler_state
{
  Texture = <g_diffuseMap>;
  MinFilter = LINEAR;
  MagFilter = LINEAR;
  MipFilter = LINEAR;
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
  output.position = mul(float4(input.position, 1.0f), g_WVP);
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
  uv.xy = uv.xy + float2(0.5f / 1280.0f, 0.5f / 720.0f);


  return saturate(input.diffuse * tex2D(g_diffSampler, uv)) +
         saturate(input.specular * tex2D(g_specSampler, uv));
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
    vertexshader = compile vs_3_0 VSRenderToScreen();
    pixelshader = compile ps_3_0 PSRenderToScreen();
  }
}