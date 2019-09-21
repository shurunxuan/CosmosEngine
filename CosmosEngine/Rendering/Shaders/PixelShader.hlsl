struct VertexToPixel
{
	float4 position				: SV_POSITION;
	float3 normal               : NORMAL;
	float2 uv                   : TEXCOORD0;
	float3 fragColor            : COLOR;
};

cbuffer colorData : register(b1, space1)
{
    float4 color;
};

float4 main(VertexToPixel input) : SV_TARGET0
{
    float ndl = dot(input.normal, normalize(float3(1.0, 1.0, 0.0)));
    return float4(color.rgb * ndl, 1.0);
}