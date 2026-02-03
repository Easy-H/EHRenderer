#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "../EasyH/Singleton.hpp"
#include <unordered_map>

class ImporterBase;

class ResourceManager : public EasyH::Singleton<ResourceManager>
{
public:
	void SetImportOption(char** extensions, int cnt, ImporterBase* importer);
	void Import();
private:
	std::unordered_map<char*, ImporterBase*> importerMap;
};

#endif