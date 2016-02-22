TextureCube baseTexture : register (t0);
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
	return baseTexture.Sample(filter, float3(input.textureCoords,1));
}