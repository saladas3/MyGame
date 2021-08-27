#pragma once

#include "Prerequisites.h"

class Resource
{

public:
	Resource(const wchar_t* full_path);
	virtual ~Resource();

protected:
	std::wstring mFullPath;

};
