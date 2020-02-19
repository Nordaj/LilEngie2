struct VSInput
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

cbuffer Model : register(b1)
{
	float4x4 model;
}

VSOutput main(in VSInput i)
{
	VSOutput o;
	o.position = mul(model, i.position);
	o.texCoord = i.texCoord;
	return o;
}
