#ifndef _SHADERSTAREGYBASE_H_
#define _SHADERSTAREGYBASE_H_

class ShaderBaseBase;

class ShaderStrategyBase {
public:
	virtual int Bind(ShaderBaseBase* targetShader) = 0;
	virtual void SetSlot(ShaderBaseBase* targetShader, int slotIdx) = 0;

};

#endif