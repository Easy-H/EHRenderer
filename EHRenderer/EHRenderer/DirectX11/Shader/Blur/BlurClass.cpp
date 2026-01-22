#include "BlurClass.hpp"

#include "BlurShaderClass.hpp"

#include "../../D3DClass.hpp"
#include "../../Object/CameraClass.hpp"
#include "../../Data/RenderTextureClass.hpp"
#include "../../Data/OrthoWindowClass.hpp"
#include "../Texture/TextureshaderClass.hpp"

BlurClass::BlurClass()
{
}

BlurClass::BlurClass(const BlurClass&)
{
}

BlurClass::~BlurClass()
{

}

bool BlurClass::Initialize(D3DClass* direct3D,
	int downSampleWidth, int downSampleHeight,
	float screenNear, float screenDepth,
	int renderWidth, int renderHeight)
{
	_downSampleWidth = downSampleWidth;
	_downSampleHeight = downSampleHeight;

	_downSampleTexture1 = make_unique<RenderTextureClass>();

	if (!_downSampleTexture1->Initialize(direct3D->GetDevice(),
		_downSampleWidth, _downSampleHeight, screenDepth, screenNear, 1)) {
		return false;
	}

	_downSampleTexture2 = make_unique<RenderTextureClass>();

	if (!_downSampleTexture2->Initialize(direct3D->GetDevice(),
		_downSampleWidth, _downSampleHeight, screenDepth, screenNear, 1)) {
		return false;
	}

	_downSampleWindow = make_unique<OrthoWindowClass>();

	if (!_downSampleWindow->Initialize(direct3D->GetDevice(), _downSampleWidth, _downSampleHeight))
	{
		return false;
	}

	_upSampleWindow = make_unique<OrthoWindowClass>();

	if (!_upSampleWindow->Initialize(direct3D->GetDevice(), renderWidth, renderHeight))
	{
		return false;
	}

	return true;
}


bool BlurClass::BlurTexture(D3DClass* Direct3D, CameraClass* Camera, RenderTextureClass* RenderTexture, TextureShaderClass* TextureShader, BlurShaderClass* BlurShader)
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float blurType;
	bool result;

	// Get the world and view matrix.
	Direct3D->GetWorldMatrix(worldMatrix);
	Camera->GetViewMatrix(viewMatrix);

	// Begin 2D rendering and turn off the Z buffer.
	Direct3D->TurnZBufferOff();

	/////////////////////////////////////////////
	// STEP 1: Down sample the render to texture.
	/////////////////////////////////////////////

	_downSampleTexture1->SetRenderTarget(Direct3D->GetDeviceContext());
	_downSampleTexture1->ClearRenderTarget(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	_downSampleTexture1->GetOrthoMatrix(orthoMatrix);

	_downSampleWindow->Render(Direct3D->GetDeviceContext());

	result = TextureShader->Render(Direct3D->GetDeviceContext(), _downSampleWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	/////////////////////////////////////////////////////////////////
	// STEP 2: Perform a horizontal blur on the down sampled texture.
	/////////////////////////////////////////////////////////////////

	// Set the blur type to zero for a horizontal blur from the blur shader.
	blurType = 0.0f;

	_downSampleTexture2->SetRenderTarget(Direct3D->GetDeviceContext());
	_downSampleTexture2->ClearRenderTarget(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	_downSampleTexture2->GetOrthoMatrix(orthoMatrix);

	_downSampleWindow->Render(Direct3D->GetDeviceContext());

	if (!BlurShader->Render(Direct3D->GetDeviceContext(), _downSampleWindow->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, _downSampleTexture1->GetShaderResourceView(),
		_downSampleWidth, _downSampleHeight, blurType))
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////
	// STEP 3: Perform a vertical blur on the horizontally blurred texture.
	//////////////////////////////////////////////////////////////////////

	// Set the blur type to one for a vertical blur from the blur shader.
	blurType = 1.0f;

	_downSampleTexture1->SetRenderTarget(Direct3D->GetDeviceContext());
	_downSampleTexture1->ClearRenderTarget(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	_downSampleTexture1->GetOrthoMatrix(orthoMatrix);

	_downSampleWindow->Render(Direct3D->GetDeviceContext());

	if (!BlurShader->Render(Direct3D->GetDeviceContext(), _downSampleWindow->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, _downSampleTexture2->GetShaderResourceView(),
		_downSampleWidth, _downSampleHeight, blurType))
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////
	// STEP 4: Up sample the blurred result.
	//////////////////////////////////////////////////////////////////////

	RenderTexture->SetRenderTarget(Direct3D->GetDeviceContext());
	RenderTexture->ClearRenderTarget(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	RenderTexture->GetOrthoMatrix(orthoMatrix);

	_upSampleWindow->Render(Direct3D->GetDeviceContext());

	if (!TextureShader->Render(Direct3D->GetDeviceContext(), _upSampleWindow->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, _downSampleTexture1->GetShaderResourceView()))
	{
		return false;
	}

	// Re-enable the Z buffer after 2D rendering complete.
	Direct3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.  And reset the viewport back to the original.
	Direct3D->SetBackBufferRenderTarget();
	Direct3D->ResetViewport();

	return true;
}