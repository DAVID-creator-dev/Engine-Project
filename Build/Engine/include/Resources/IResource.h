#pragma once
#include <string>

class IResource
{
public:
	virtual ~IResource() = default;
	virtual void LoadResource(const char* path) = 0;

	virtual std::string GetPath() const { return _path; }

protected:
    std::string _path;
}; 
