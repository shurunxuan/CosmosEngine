// Constant Buffer
// - Allows us to define a buffer of individual variables 
//    which will (eventually) hold data from our C++ code
// - All non-pipeline variables that get their values from 
//    our C++ code must be defined inside a Constant Buffer
// - The name of the cbuffer itself is unimportant
cbuffer ubo : register(b0)
{
	matrix model;
	matrix view;
	matrix proj;
	matrix itModel;
};

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT0;
};

struct VertexToPixel
{
	float4 position				: SV_POSITION;
	float3 normal               : NORMAL;
	float2 uv                   : TEXCOORD0;
	float3 fragColor            : COLOR;
};

VertexToPixel main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixel output;

	// Then we convert our 3-component position vector to a 4-component vector
	// and multiply it by our final 4x4 matrix.
	//
	// The result is essentially the position (XY) of the vertex on our 2D 
	// screen and the distance (Z) from the camera (the "depth" of the pixel)
	float4 worldPos = mul(float4(input.position, 1.0f), model);
	float4 viewPos = mul(worldPos, view);
	output.position = mul(viewPos, proj);
	output.normal = normalize(mul(float4(input.normal, 0.0), itModel).xyz);
	output.uv = input.uv;
	output.fragColor = float3(1.0, 1.0, 1.0);
	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}