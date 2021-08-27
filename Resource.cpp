#include "Resource.h"

Resource::Resource(const wchar_t* full_path)
{
	this->mFullPath = full_path;
}

Resource::~Resource() = default;
