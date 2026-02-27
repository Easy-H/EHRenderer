cbuffer MatrixBuffer {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ClipPlaneBuffer {
	float4 clipPlane;
};

struct VertexInputType {
	float3 position : POSITION;
	float2 tex: TEXCOORD0;
};

struct PixelInputType {
	float4 position: SV_POSITION;
	float2 tex : TEXCOORD0;
	float clip: SV_ClipDistance0;
};

PixelInputType ClipPlaneVertexShader(VertexInputType input) {
	PixelInputType output;
	float4 inputPosition = float4(input.position, 1.f);

	output.position = mul(inputPosition, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.clip = dot(mul(inputPosition, worldMatrix), clipPlane);

	return output;
}