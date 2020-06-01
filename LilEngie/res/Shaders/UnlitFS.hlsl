cbuffer PerMaterial : register(b3)
{
	float3 color;
}

struct VSOutput
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(in VSOutput i) : SV_TARGET
{
	return float4(color, 1);
}
