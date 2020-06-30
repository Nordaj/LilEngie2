cbuffer PerMaterial : register(b3)
{
	float3 color;
}

sampler samp;
Texture2D<float4> tex;

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(in VSOutput i) : SV_TARGET
{
	return tex.Sample(samp, i.texCoord) * float4(color, 1);
}
