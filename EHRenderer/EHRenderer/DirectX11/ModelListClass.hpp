#pragma once

#include <memory>

using namespace std;

class ModelListClass {
private:
	struct ModelInfoType {
		float positionX;
		float positionY;
		float positionZ;
	};
public:
	ModelListClass();
	ModelListClass(const ModelListClass&);
	~ModelListClass();
	
	void Initialize(int);
	
	int GetModelCount();
	void GetData(int, float&, float&, float&);

private:
	int _modelCount;
	unique_ptr<ModelInfoType[]> _modelInfoList;
};