#pragma pack_matrix(row_major)

struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float4 color : COLOR;
	float2 uv : TEXTURE;
	float3 normal : NORMAL;
};

struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 textureCoords : TEXCOORD;
	float3 normal : NORMAL;
};

cbuffer MVP : register(b0)
{
	float4x4 MVP;
	float4x4 WORLDMATRIX;
};

OUTPUT_VERTEX main( INPUT_VERTEX Input )
{
	OUTPUT_VERTEX Output = (OUTPUT_VERTEX)0;
	Output.position.w = 1;

	float4 vertex = float4(Input.coordinate.xyz, 1);

	//vertex = mul(vertex, WORLDMATRIX);
	vertex = mul(vertex, MVP);
	
	Output.position = vertex.xyww;
	Output.textureCoords = Input.coordinate.xyz;
	return Output;
}