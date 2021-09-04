Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
};

// Function
float2 distort(float2 p)
{
	// atan2 - used to convert carthesian coordinates to polar coordinates 
	//	(polar coordinates are not defined by 2 points, they are defined by a radius and an angle)
	// atan2(y, x) -> will return the angle
	float theta = atan2(p.y, p.x);
	float radius = length(p);

	// By modifying the radius we create the distortion effect
	radius = pow(radius, 2.0);

	// After applying the effect we must convert back from polar to carthesian coords
	p.x = radius * cos(theta);
	p.y = radius * sin(theta);

	// Convert the range from [-1, 1] to [0, 1] ([-1, 1] + 1 / 2)
	return 0.5 * (p + 1.0);
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float2 xy = 2.0 * input.texcoord.xy - 1.0;
	float2 uv;

	float d = length(xy);

	if (d < 1.0) {
		// Inside the circle
		uv = distort(xy);
	}
	else {
		// Outside the circle
		uv = input.texcoord.xy;
	}
	
	return Texture.Sample(TextureSampler, uv);
}