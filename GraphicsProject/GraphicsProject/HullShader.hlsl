// Input control point
struct VS_CONTROL_POINT_OUTPUT
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

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

cbuffer Constant : register (b0)
{
	float3 camPos;
	float pad;
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	float max = 3.0f;
	float min = 0.0f;
	float dis = length(ip[PatchID].position - camPos);

	dis /= max;
	int tessFactor = 128;
	tessFactor = (int)clamp(tessFactor /*- (dis * (float)tessFactor)*/, 1, 128);

	// Insert code to compute Output here
	Output.EdgeTessFactor[0] = 
	Output.EdgeTessFactor[1] = 
	Output.EdgeTessFactor[2] = 
	Output.InsideTessFactor = tessFactor; // e.g. could calculate dynamic tessellation factors instead

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.position = ip[i].position;
	Output.color = ip[i].color;
	Output.textureCoords = ip[i].textureCoords;
	Output.normal = ip[i].normal;
	return Output;
}
