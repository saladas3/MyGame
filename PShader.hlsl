// Retrieve the constant buffer set in the code
cbuffer constant: register(b0)
{
	unsigned int m_time;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

// float4 -> red green blue alpha
float4 psmain(PS_INPUT input) : SV_TARGET
{
	return float4(lerp(input.color, input.color1, (sin(m_time / 500.0f) + 1.0f) / 2.0f), 1.0f);
}