struct VertexToPixel
{
	float4 position				: SV_POSITION;
	float3 normal               : NORMAL;
	float2 uv                   : TEXCOORD0;
	float3 fragColor            : COLOR;
};

cbuffer colorData : register(b1)
{
    float4 color;
};

cbuffer textureData : register(b2)
{
    int hasDiffuse;
};

Texture2D diffuseTexture  : register(t3);
SamplerState diffuseSampler : register(s4);

float4 main(VertexToPixel input) : SV_TARGET0
{
    float ndl = dot(input.normal, normalize(float3(1.0, 1.0, 0.0)));
    float3 tex = float3(1.0, 1.0, 1.0);
    if (hasDiffuse == 1)
    {
        tex = diffuseTexture.Sample(diffuseSampler, input.uv).rgb;
    }
    //return float4(color.rgb * ndl, 1.0 - tex.r);
    return float4(tex * input.fragColor * (ndl * 0.5 + 0.5), 1.0);
}