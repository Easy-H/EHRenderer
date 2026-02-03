#ifndef _SHADERBASE_H_
#define _SHADERBASE_H_

class ShaderBaseBase {
public:
	virtual ~ShaderBaseBase() {}

	virtual bool Initialize() = 0;
	virtual bool Render(int indexCount) = 0;

	virtual void BindTexture(int slot, int textureId) = 0;
};

#endif