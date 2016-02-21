#pragma pack_matrix(row_major)
texture2D heightMap : register (t0);
SamplerState samHeightmap {
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct DS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 textureCoords : TEXCOORD;
	float3 normal : NORMAL;
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 textureCoords : TEXCOORD;
	float3 normal : NORMAL;
};

cbuffer	constantBuffer : register (b0)
{
	float4x4 MVP;
	float4x4 WORLDMATRIX;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3] : SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor : SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{

	DS_OUTPUT Output;
	float4 test = (heightMap.SampleLevel(samHeightmap, float2(patch[0].textureCoords*domain.x + patch[1].textureCoords*domain.y + patch[2].textureCoords*domain.z), 0)).r;

	Output.position = float4(patch[0].position.xyz * domain.x + patch[1].position.xyz * domain.y + patch[2].position.xyz * domain.z, 1);
	Output.position.y = test.r * 100 ;
	Output.position = mul(Output.position, WORLDMATRIX);
	Output.position = mul(Output.position, MVP);

	Output.normal = float3(patch[0].normal*domain.x + patch[1].normal*domain.y + patch[2].normal*domain.z);

	Output.textureCoords = float2(patch[0].textureCoords*domain.x + patch[1].textureCoords*domain.y + patch[2].textureCoords*domain.z);

	Output.color = patch[0].color;

	return Output;
}
