#include "ResourceManager.h"

ResourceManager::ResourceManager() = default;

ResourceManager::~ResourceManager() = default;

ResourcePtr ResourceManager::createResourceFromFile(const wchar_t* filepath)
{
	std::wstring full_path = std::filesystem::absolute(filepath);
	auto it = mResourcesMap.find(full_path);

	if (it != mResourcesMap.end())
	{
		return it->second;
	}

	Resource* raw_res = this->createResourceFromFileConcrete(full_path.c_str());

	if (raw_res)
	{
		ResourcePtr res(raw_res);
		mResourcesMap[full_path] = res;
		return res;
	}

	return nullptr;
}
