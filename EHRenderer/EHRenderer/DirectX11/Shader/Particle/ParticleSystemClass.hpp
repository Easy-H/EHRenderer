#pragma once;

#include <d3d11.h>
#include <directxmath.h>
#include <memory>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

class TextureClass;

class ParticleSystemClass {
private:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};

	struct ParticleType {
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;

	};

public:
	ParticleSystemClass();
	ParticleSystemClass(const ParticleSystemClass&);
	~ParticleSystemClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	bool Frame(float, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

private:
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	
	bool InitializeParticleSystem();

	bool InitializeBuffers(ID3D11Device*);
	void RenderBuffers(ID3D11DeviceContext*);

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();
	bool UpdateBuffers(ID3D11DeviceContext*);

	unique_ptr<TextureClass> _texture;
	unique_ptr<ParticleType[]> _particleList;

	unique_ptr<VertexType[]> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;

	int _vertexCount;
	int _indexCount;

	float _particleDeviationX, _particleDeviationY, _particleDeviationZ;
	float _particleVelocity, _particleVelocityVariation;
	float _particleSize, _particlePerSecond;
	int _maxParticles;

	int _currentParticleCount;
	float _accumulatedTime;

};