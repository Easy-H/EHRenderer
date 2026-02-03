#ifndef _MODELBASE_H_
#define _MODELBASE_H_

class ModelBase {
public:
	virtual bool Initialize(char* addr) = 0;
	virtual bool Bind() = 0;
};

#endif