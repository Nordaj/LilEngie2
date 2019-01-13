struct VSOutput
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(in VSOutput i) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
