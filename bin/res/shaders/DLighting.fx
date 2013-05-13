// Deferred lighting shader.

#define AMBIENT_LIGHT 1
#define DIRECTIONAL_LIGHT 2
#define POINT_LIGHT 3
#define SPOT_LIGHT 4


cbuffer perFrame
{
	float4x4 g_view;
	float4x4 g_invView;
	float4x4 g_invProjection;

	// Used to offset textures.
	float2 g_halfPixel;
	
	texture g_normalMap;
	texture g_depthMap;
};

cbuffer perLight
{
	// Holds an integer that represents light types.
	uint g_lightType;

	// Used for light volume.
	float4x4 g_WVP;

	// Lights world position - used by pointlights and spotlights.
	float3 g_lightWP;

	// Direction of light - used by directionallights and spotlights.
	float3 g_lightDir;

	// Light color - Used by all lights.
	float4 g_lightColor; 

	// Attenuation factors, used by pointlights.
	float g_att0;
	float g_att1;
	float g_att2;
};

// Used to sample the normal map.
sampler g_normalSampler = sampler_state
{
	Texture = g_normalMap;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
};

// Used to sample depth map.
sampler g_depthSampler = sampler_state
{
	Texture = g_depthMap;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
};


// Helper function to retrieve a view-space position from a screen-space coordinate and depth.
float3 UnProjectPosition(float2 uv)
{
  float z = tex2D(g_depthSampler, uv).r;
  float x = uv.x * 2 - 1;
  float y = (1 - uv.y)  * 2 -1;

  // Un-Projected position.
  float4 upPos = mul(float4(x, y, z, 1.0f), g_invProjection);
  return upPos.xyz / upPos.w;
}

// Used to calculate a directional light.
float4 DirectionalLight(float3 position, float3 normal, float3 lightDir, float specExp)
{
	// Return value.
	float4 retCol = 0;

	// Diffuse intensity.
	float dInten = max(0.0f, dot(normal, -lightDir));
	
	if(dInten > 0)
	{
		float3 viewVec = normalize(-position);
		float3 reflectVec = reflect(lightDir, normal);

		// Specular intensity
		float sInten = pow(max(0.0f, dot(viewVec, reflectVec)), specExp);
		sInten = 0;
		retCol = float4(g_lightColor.rgb * dInten, sInten);
	}

	return retCol;
}

float4 PointLight(float3 position, float3 normal, float3 lightPos, float specExp)
{
	// Return value.
	float4 retCol = 0;

	// Distance between fragment position and light.
	float lightDist = distance(lightPos, position);

	// Direction from light to position.
	float3 lightDir = normalize((lightPos - position));
	
	// Diffuse intensity.
	float dInten = max(0.0f, dot(lightDir, normal));

	if(dInten > 0)
	{
		// Attenuation.
		float att = 1.0f / (g_att0 + g_att1 * lightDist + g_att2 * lightDist * lightDist);

		float3 viewVec = normalize(-position);
		float3 refVec = reflect(-lightDir, normal);

		// Specular intensity.
		float sInten = pow(max(0.0f, dot(viewVec, refVec)), specExp);

		retCol = float4(dInten * att * g_lightColor.rgb, sInten * att);
	}

	return retCol;
}


// Vertex Shader Input.
struct VSInput
{
	float4 position : POSITION0;
};

// Input to pixel shader.
struct PSInput
{
	// Required output for vertex shader.
	float4 SVPos : SV_POSITION;

	// Holds same as SVPos, but accessible for pixel shader.
	float4 position : TEXCOORD0;

	// Light position transformed into view space.
	float3 viewLightPos : TEXCOORD1;

	// Light direction transformed into view-space
	float3 viewLightDir : TEXCOORD2;
};

// Vertex shader function.
PSInput VS(VSInput input)
{
	PSInput pOut = (PSInput)0;

	// Simple transform into screen coordinates, used by all lights.
	pOut.SVPos = mul(float4(input.position.xyz, 1.0f), g_WVP);
	pOut.position = pOut.SVPos;

	// Handle specific parameters for different light types. We don't need to
	// do anything for ambient lights.
	switch(g_lightType)
	{
		case DIRECTIONAL_LIGHT:
		// Transform light direction into view space.
		pOut.viewLightDir = normalize(mul(float4(g_lightDir, 0), g_invView).xyz);
		break;
		case POINT_LIGHT:
		// Transform the lights world position into view space.
		pOut.viewLightPos = mul(float4(g_lightWP, 1.0f), g_view).xyz;
		break;
		case SPOT_LIGHT:
		// TODO , Not implemented yet.
		break;

	}

	return pOut;
};

// Pixel shader function, returns lit color in RGB and specular intensity in A channel.
float4 PS(PSInput input) : SV_TARGET
{
	// For ambient lights we can just return early with the color.
	if(g_lightType == AMBIENT_LIGHT)
	{
		return g_lightColor;
	}

	// Will be our return value.
	float4 litColor = float4(0.25f, 0, 0.25f, 1.0f);

	// Calculated texture coordinates from screen position.
	float2 uv = (input.position.xy / input.position.w) * 0.5f + 0.5f;
	uv.y = 1.0f - uv.y;
	uv += g_halfPixel;

	// Retrieve value from normal map - contains normal in rgb, and specular exponent in alpha.
	float4 normalVal = tex2D(g_normalSampler, uv);

	float specExp = max(normalVal.a * 255, 1.0f);

	// Bring normal back into [-1. 1] range.
	float3 normal = normalize(2.0f * normalVal.xyz - 1.0f);

	// Retrieve view-space position.
	float3 position = UnProjectPosition(uv);

	if(g_lightType == DIRECTIONAL_LIGHT)
	{
		litColor = DirectionalLight(position, normal, input.viewLightDir, specExp);
	}
	else
	{
		litColor = PointLight(position, normal, input.viewLightPos, specExp);
	}

	return litColor;
}

technique LightTech
{
	pass p0
	{
		vertexShader = compile vs_3_0 VS();
		pixelShader = compile ps_3_0 PS();

		AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = ONE;
		BlendOp = ADD;
		//FillMode = WIREFRAME;
		CullMode = NONE;
	}
}