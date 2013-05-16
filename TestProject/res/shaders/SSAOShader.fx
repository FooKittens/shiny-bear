// Screen Space Ambient Occlusion shader.


#define KERNELSIZE 8


cbuffer once
{
	texture g_noiseMap;
	texture g_depthMap;
	texture g_normalMap;
	float2 g_halfPixel;
	float2 g_dimensions;

	float4x4 g_projection;
	float4x4 g_invProjection;
	float g_zFar;
};

sampler g_noiseSampler = sampler_state
{
	Texture = g_noiseMap;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	//AddressU = WRAP;
	//AddressV = WRAP;
};

sampler g_depthSampler = sampler_state
{
	Texture = g_depthMap;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;	
	AddressU = CLAMP;
	AddressV = CLAMP;
};

sampler g_normalSampler = sampler_state
{
	Texture = g_normalMap;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	//AddressU = CLAMP;
	//AddressV = CLAMP;	
};

struct PSInput
{
	float4 SVPos : SV_POSITION;
	float4 pos : TEXCOORD0;
};

// Hopefully random enough..
float random(float a, float b)
{
	float res = frac(sin(dot(float2(a, b), float2(12.32424f, 4.234424f)))) * 438.3242111f;
	return res;
}

// Helper function to retrieve a view-space position from a screen-space coordinate and depth.
float3 UnProjectPosition(float depth, float4 screenPos)
{
	float3 posView = mul(screenPos, g_invProjection).xyz;
	float3 viewRay = float3(posView.xy * (g_zFar / posView.z), g_zFar);
	return viewRay * depth;
}


PSInput VS(float4 inPos : POSITION0)
{
	PSInput pOut;
	pOut.SVPos = float4(inPos.xyz, 1.0f);
	pOut.pos = float4(inPos.xyz, 1.0f);
	return pOut;
}

float4 PS(PSInput input) : SV_TARGET
{
	// kernel
	float3 kernel[KERNELSIZE] = 
	{
		float3(+1, +1, +1) * 0.2419,
		float3(-1, -1, +1) * 0.042,
		float3(-1, +1, +1) * 0.094,
		float3(+1, -1, +1) * 0.952,

		float3(+1, +1, +1) * 0.746,
		float3(-1, -1, +1) * 0.11,
		float3(+1, -1, +1) * 0.2234,
		float3(-1, +1, +1) * 0.4928,
	};

	for(int i = 0; i < KERNELSIZE; ++i)
	{
		float scale = float(i) / float(KERNELSIZE);
		scale = lerp(0.1f, 1.0f, scale * scale);

		float2 val = float2(i - KERNELSIZE / 2, (i * 7) - KERNELSIZE / 2);

		float x = noise(val * 352.0f) + 0.001f;
		float y = noise(val * 11) +0.001f;
		float z = abs(noise(val * -1.0f * 7 * 37)) + 0.001f;

		kernel[i] = normalize(float3(x, y, z));
		kernel[i] *= scale;
	}	

	// Calculated texture coordinates from screen position.
	float2 uv = (input.pos.xy / input.pos.w) * 0.5f + 0.5f;
	uv.y = 1.0f - uv.y;
	uv += g_halfPixel;

	float2 noiseScale = g_dimensions / float2(4, 4);

	float4 normalVal = tex2D(g_normalSampler, uv);
	float4 noiseVal = tex2D(g_noiseSampler, uv * noiseScale);

	float3 normal = normalize(2.0f * normalVal.xyz - 1.0f);
	float3 randVec = normalize(2.0f * noiseVal.xyz - 1.0f);

	float fragDepth = tex2D(g_depthSampler, uv).r;
	float3 viewPos = UnProjectPosition(fragDepth, input.pos);


	//return float4(normal, 1.0f);
	//return noiseVal;
	//return float4(randVec, 0); //normalVal;

	float3 tangent = normalize(randVec - normal * dot(randVec, normal));
	float3 bitangent = cross(normal, tangent);

	float3x3 tbn = float3x3(tangent, bitangent, normal);

	tbn = transpose(tbn);
	//tbn = float3x3(
	//	float3(1, 0, 0),
	// 	float3(0, 1, 0),
 	// 	float3(0, 0, 1)
 	//);

	//tbn = float3x3(
	//	float3(tangent.x, bitangent.x, normal.x),
	// 	float3(tangent.y, bitangent.y, normal.y),
 	// 	float3(tangent.z, bitangent.z, normal.z)
 	//);
	
	float occlusion = 0.0f;

	// Move for experimentation.
	float RADIUS = 0.25f;

	
	for(int i = 0; i < KERNELSIZE; ++i)
	{
		float3 samp = mul(kernel[i], tbn);

		samp = normalize(samp) * RADIUS  + viewPos;
		
		//return float4(samp, 1.0f);

		float4 offset = float4(samp, 1.0f);
		offset = mul(offset, g_projection);
		

		//return offset;
		offset.xyz /= offset.w;
		offset.xy = offset.xy * 0.5f + 0.5f;
		offset.y = 1.0f - offset.y;
		
		offset.xy += g_halfPixel;

		//return float4(offset.xy, 0, 1.0f);		

		float sampDepth = tex2D(g_depthSampler, offset.xy).r;

		//return float4(sampDepth, sampDepth, sampDepth, 1.0f);

		float rangeCheck = abs(viewPos.z - samp.z) < RADIUS ? 1.0 : 0.0;
		rangeCheck = 1.0f;
		occlusion += (sampDepth >= offset.z ? 1.0 : 0.0) * rangeCheck;
	}
	occlusion = (occlusion / float(KERNELSIZE));		
	return float4(occlusion, occlusion, occlusion, 1.0f);
}

technique SSAOTech
{
	pass p0
	{
		vertexShader = compile vs_3_0 VS();
		pixelShader = compile ps_3_0 PS();
	}
}