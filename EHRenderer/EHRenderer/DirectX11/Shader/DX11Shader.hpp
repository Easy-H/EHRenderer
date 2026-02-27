#ifndef _DX11SHADER_H_
#define _DX11SHADER_H_

#include "DX11ShaderBase.hpp"
#include <unordered_map>
#include <memory>
#include <wrl.h>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class DX11Shader : public DX11ShaderBase {
	struct MatrixBufferType {
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
	struct SlotInfor {
		unsigned int slotIdx = -1;
		size_t size = -1;
		bool isReset = false;
		ComPtr<ID3D11Buffer> slot;
	};
public:
	virtual bool Initialize(ID3D11Device* device, HWND hwnd) override;
	~DX11Shader();

	virtual bool Render(int indexCount, const Transform* position) override;
	
	void SetTargetVS(WCHAR* vsFilename, const char* vsFunc);
	void SetTargetPS(WCHAR* psFilename, const char* psFunc);
	virtual void GetSlot(const std::string& name, int& slotId, size_t& size) override;
	virtual void SetSlot(int slotId, int slotOffset, void* valuePtr, size_t size) override;
	virtual int GetSlotCnt() override;
	virtual void ResetSlot(int slotId, size_t size) override;

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	bool SetConstantBuffer(ID3D11Device* device,
		ID3D11ShaderReflection* reflection, D3D11_SHADER_DESC shaderDesc, int& startIdx);
	std::unordered_map<std::string, int> _slotMap;

	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11SamplerState> _sampleState;

	std::vector<std::unique_ptr<SlotInfor>> _slotData;

	int _vertexSlotStartIdx;
	int _pixelSlotStartIdx;
	int _matrixBufferSlot = 0;

	WCHAR* _vsFilename;
	WCHAR* _psFilename;
	const char* _vsFunc;
	const char* _psFunc;
};

#endif