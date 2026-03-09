#ifndef _DX11SHADER_H_
#define _DX11SHADER_H_

#include "DX11ShaderBase.hpp"
#include "ShaderStrategy/ShaderStrategyBase.hpp"
#include <unordered_map>
#include <unordered_set>
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
	struct StrategyInfo {
		int slotId;
		ShaderStrategyBase* strategy;
	};
public:
	virtual bool Initialize(ID3D11Device* device, HWND hwnd) override;
	~DX11Shader();

	virtual bool Render(int indexCount, const Transform* position) override;
	
	void SetTargetVS(WCHAR* vsFilename, const char* vsFunc);
	void SetTargetPS(WCHAR* psFilename, const char* psFunc);
	void AddStrategy(ShaderStrategyBase* strategy);
	virtual int GetSlotIdx(const std::string& name) override;
	virtual void GetSlot(int slotId, size_t& size) override;
	virtual void SetSlot(int slotId, int slotOffset, void* valuePtr, size_t size) override;
	virtual int GetSlotCnt() override;
	virtual void ResetSlot(int slotId, size_t size) override;

protected:
	virtual void GetShaderParameter(const Transform* position);
	virtual void SetShaderParameter(ID3D11DeviceContext* deviceContext);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	bool SetConstantBuffer(ID3D11Device* device,
		ID3D11ShaderReflection* reflection, D3D11_SHADER_DESC shaderDesc, int& startIdx);

	std::unordered_map<std::string, int> _slotMap;
	std::vector<std::unique_ptr<SlotInfor>> _slotInfo;
	std::vector<std::unique_ptr<StrategyInfo>> _strategyInfo;
	std::unordered_set<int> _strategySet;

	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11SamplerState> _sampleState;

	int _vertexSlotStartIdx;
	int _pixelSlotStartIdx;
	int _matrixBufferSlot = 0;

	WCHAR* _vsFilename;
	WCHAR* _psFilename;
	const char* _vsFunc;
	const char* _psFunc;
};

#endif