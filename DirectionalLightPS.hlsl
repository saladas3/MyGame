Texture2D Color: register(t0);
sampler ColorSampler: register(s0);

Texture2D Occlusion: register(t1);
sampler OcclusionSampler: register(s1);

Texture2D Normal: register(t2);
sampler NormalSampler: register(s2);

Texture2D Emissive: register(t3);
sampler EmissiveSampler: register(s3);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 direction_to_camera: TEXCOORD1;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;

	float4 m_light_direction;
	float4 m_camera_position;

	float4 m_light_position;
	float m_light_radius;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 color = Color.Sample(ColorSampler, float2(input.texcoord.x, input.texcoord.y));
	float4 occlusion = Occlusion.Sample(OcclusionSampler, float2(input.texcoord.x, input.texcoord.y));
	float4 normal = Normal.Sample(NormalSampler, float2(input.texcoord.x, input.texcoord.y));
	float4 emissive = Emissive.Sample(EmissiveSampler, float2(input.texcoord.x, input.texcoord.y));

	color = color + emissive;

	// Ambient light
	float ka = 8.5;
	float3 ia = float3(0.09, 0.082, 0.082);
	ia *= (color.rgb);

	float3 ambient_light = ka * ia;

	// Diffuse light
	float kd = 0.7;
	float amount_diffuse_light = dot(m_light_direction.xyz, input.normal);
	float3 id = float3(1, 1, 1);
	id *= (color.rgb);

	float3 diffuse_light = kd * id * amount_diffuse_light;

	// Specular light
	float ks = 0.0;
	float3 is = float3(1.0, 1.0, 1.0);
	float3 reflected_light = reflect(m_light_direction.xyz, input.normal);
	float shininess = 30.0;
	float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);
	float3 specular_light = ks * amount_specular_light * is;

	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light, 1.0);
}