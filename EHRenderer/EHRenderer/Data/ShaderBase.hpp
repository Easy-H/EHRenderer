#ifndef _SHADERBASE_H_
#define _SHADERBASE_H_

#include <string>

class Transform;

class ShaderBaseBase {
public:
	virtual ~ShaderBaseBase() {}

	virtual bool Render(int indexCount, const Transform* position) = 0;
	virtual void BindTexture(int slot, int textureId) = 0;

	virtual void GetSlot(const std::string& name, int& slotId, size_t& size) = 0;
	virtual void SetSlot(int slotId, int slotOffset, void* valuePtr, size_t size) = 0;
	virtual int GetSlotCnt() = 0;
	virtual void ResetSlot(int slotId, size_t size) = 0;
};

#endif