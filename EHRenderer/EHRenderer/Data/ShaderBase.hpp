#ifndef _SHADERBASE_H_
#define _SHADERBASE_H_

class Transform;

class ShaderBaseBase {
public:
	virtual ~ShaderBaseBase() {}

	virtual bool Render(int indexCount, const Transform* position) = 0;
	virtual void BindTexture(int slot, int textureId) = 0;
};

#endif