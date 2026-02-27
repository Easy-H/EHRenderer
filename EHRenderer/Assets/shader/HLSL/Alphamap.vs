cbuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType {
	float3 position: POSITION;
	float2 tex: TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType {
	float4 position: SV_POSITION;
	float2 tex: TEXCOORD0;
};

PixelInputType AlphaMapVertexShader(VertexInputType input) {
	PixelInputType output;

	output.position = mul(float4(input.position, 1.f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	return output;
}