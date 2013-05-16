	for(int i = 0; i < KERNELSIZE; ++i)
	{
		float scale = float(i) / float(KERNELSIZE);
		scale = lerp(0.1f, 1.0f, scale * scale);

		float x = noise(float2(i * -5.0f , i * i + 7)) + 0.001f;
		float y = noise(float2(i * 13 + 7, 99 + i * i * (i + 7))) +0.001f;
		float z = abs(noise(float2(i + 3 * 7, i * i * (i - 7) + 77)) + 0.001f);

		kernel[i] = normalize(float3(x, y, z));
		kernel[i] *= scale;
	}

	// kernel
	float3 kernel[KERNELSIZE] = 
	{
		float3(+1, +1, +1) * 0.2419,
		float3(-1, -1, +1) * 0.042,
		float3(-1, +1, +1) * 0.094,
		float3(+1, -1, +1) * 0.952,

		float3(+1, +1, +0) * 0.746,
		float3(-1, -1, +0) * 0.11,
		float3(+1, -1, +0) * 0.2234,
		float3(-1, +1, +0) * 0.4928,
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
