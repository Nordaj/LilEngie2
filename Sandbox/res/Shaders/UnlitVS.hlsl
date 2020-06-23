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

cbuffer PerObject : register(b2)
{
	float4x4 model;
}

cbuffer PerCamera : register(b1)
{
    float4x4 vp;
}

VSOutput main(in VSInput i)
{
	VSOutput o;
	o.position = mul(mul(vp, model), i.position);
	o.texCoord = i.texCoord;
	return o;
}
