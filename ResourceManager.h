#pragma once

#include "Resource.h"

class ResourceManager
{

public:
	ResourceManager();
	virtual ~ResourceManager();

	ResourcePtr createResourceFromFile(const wchar_t* filepath);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filepath) = 0;

private:
	std::unordered_map<std::wstring, ResourcePtr> mResourcesMap;

};

