#pragma once
#include <string>
#include "Utils/ResourceParams.h"

class IResource
{
public:
	virtual ~IResource() = default;
	virtual void LoadResource(const char* path, const ResourceLoadParams& params = {}) = 0;

	virtual std::string GetPath() const { return _path; }
	virtual void SetPath(std::string &path) { _path = path; }

protected:
    std::string _path;
}; 
