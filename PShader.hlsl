// Need to add a sampler for the texture?
Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

// Retrieve the constant buffer set in the code
cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texcoord: TEXCOORD0;
};

// float4 -> red green blue alpha
float4 psmain(PS_INPUT input) : SV_TARGET
{
	return Texture.Sample(TextureSampler, input.texcoord);
}