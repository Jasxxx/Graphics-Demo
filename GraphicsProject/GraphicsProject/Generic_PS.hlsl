texture2D baseTexture : register (t0);
SamplerState filter : register(s0);

struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 textureCoords : TEXCOORD;
	float3 normal : NORMAL;
};

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	float4 TextureColor = baseTexture.Sample(filter, input.textureCoords.xy);

	return TextureColor;
}