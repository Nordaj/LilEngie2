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

VSOutput main(in VSInput i)
{
	VSOutput o;
	o.position = i.position;
	o.texCoord = i.texCoord;
	return o;
}
