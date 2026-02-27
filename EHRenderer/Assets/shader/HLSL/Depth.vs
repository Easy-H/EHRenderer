cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType {
	float3 position: POSITION;
};

struct PixelInputType {
	float4 position: SV_POSITION;
	float4 depthPosition : TEXCOORD0;
};

PixelInputType DepthVertexShader(VertexInputType input) {
	PixelInputType output;

	output.position = mul(float4(input.position, 1.f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.depthPosition = output.position;

	return output;
}