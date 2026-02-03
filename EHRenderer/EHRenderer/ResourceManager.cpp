#include "ResourceManager.hpp"
#include "ImporterBase.hpp"

void ResourceManager::SetImportOption(char** extensions, int cnt, ImporterBase* importer)
{
	for (int i = 0; i < cnt; i++) {
		importerMap[extensions[i]] = importer;
	}
}

void ResourceManager::Import()
{

}
