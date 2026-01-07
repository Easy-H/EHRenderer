#include "ModelListClass.hpp"

ModelListClass::ModelListClass()
{
}

ModelListClass::ModelListClass(const ModelListClass&)
{
}

ModelListClass::~ModelListClass()
{
}

void ModelListClass::Initialize(int numModels)
{
	_modelCount = numModels;
	_modelInfoList = make_unique<ModelInfoType[]>(_modelCount);

	srand((unsigned int)time(nullptr));

	for (int i = 0; i < _modelCount; i++) {
		_modelInfoList[i].positionX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.f;
		_modelInfoList[i].positionY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.f;
		_modelInfoList[i].positionZ = ((((float)rand() - (float)rand()) / RAND_MAX)) * 10.f + 5.f;

	}

}

int ModelListClass::GetModelCount()
{
	return _modelCount;
}

void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ)
{
	positionX = _modelInfoList[index].positionX;
	positionY = _modelInfoList[index].positionY;
	positionZ = _modelInfoList[index].positionZ;
}
