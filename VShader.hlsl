// Retrieve the constant buffer set in the code
cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
};

// In order to handle more than 1 attribute it is better to create data structure
// One struct for input
struct VS_INPUT
{
	// struct that hold the attributes described in vertex buffer
	float4 position : POSITION;
	float4 position1 : POSITION1;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

// One struct for output
struct VS_OUTPUT
{
	// SV_POSITION - system value semantic - written this way indicates to the graphics pipeline that the output
	// of the vertex shader will contain the final transformed vertex position in the screen space coordinates used for rasterization
	float4 position : SV_POSITION;
	// The color attributes will be passed to the pixel shader
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

// vsmain is the entry point of the vertex shader
VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//output.position = lerp(input.position, input.position1, (sin(m_time / 1000.0f) + 1.0f) / 2.0f);

	// World space - convert input to world space coord
	output.position = mul(input.position, m_world);
	// View space - convert from world to view space coord
	output.position = mul(output.position, m_view);
	// Screen space - convert from view space coord to screen space
	output.position = mul(output.position, m_proj);

	output.color = input.color;
	output.color1 = input.color1;
	
	return output;
}
