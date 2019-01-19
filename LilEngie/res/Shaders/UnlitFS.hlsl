cbuffer Color : register(b0)
{
	float4 color;
}

struct VSOutput
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(in VSOutput i) : SV_TARGET
{
	return color;
}
