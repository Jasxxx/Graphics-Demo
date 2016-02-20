#pragma pack_matrix(row_major)
StructuredBuffer<float2> changeValues : register (t0);

struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float4 color : COLOR;
	float2 uv : TEXTURE;
	float3 normal : NORMAL;
	float3 instancePos : INSTANCEPOS;
};

struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float2 textureCoords : TEXTURE;
	float3 normalOut : NORMAL;
	float4 projectedCoordinate : POSITION2;
	float3 position : POSITION;
};

// TODO: PART 3 STEP 2a
cbuffer THIS_IS_VRAM : register( b0 )
{
	float4x4 WORLDMATRIX;
	float4x4 VIEWMATRIX;
	float4x4 PROJECTIONMATRIX;
};

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer, uint id : SV_VertexID)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	sendToRasterizer.projectedCoordinate.w = 1;

	float4 vertex = float4(fromVertexBuffer.coordinate.xyz, 1);

	vertex += float4(fromVertexBuffer.instancePos, 0.0f);
	vertex.y = changeValues[id].y;
	sendToRasterizer.projectedCoordinate = float4(vertex);
	sendToRasterizer.position = vertex.xyz;
	vertex = mul(vertex, WORLDMATRIX);
	vertex = mul(vertex, VIEWMATRIX);
	vertex = mul(vertex, PROJECTIONMATRIX);
	
	sendToRasterizer.colorOut = fromVertexBuffer.color;
	sendToRasterizer.textureCoords = fromVertexBuffer.uv;

	sendToRasterizer.normalOut = mul(float4(fromVertexBuffer.normal.xyz, 0), WORLDMATRIX).xyz;

	return sendToRasterizer;
}