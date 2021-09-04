#pragma once

#include "Framework.h"
#include "obs.h"

#include <string>

MAKE_SHARED_CLASS(ObjectContext);

class TSL_EXPORT Descriptor {
private:
	const std::string mTypeName;

public:
	Descriptor(const char *mTypeName) : mTypeName(mTypeName) {}

	inline const char *GetTypeName() const { return mTypeName.c_str(); } 
};

class TSL_EXPORT SourceDescriptor : public Descriptor {
public:
	SourceDescriptor(const char *typeName) : Descriptor(typeName) {}
};

class TSL_EXPORT FilterDescriptor : public Descriptor {
public:
	FilterDescriptor(const char *typeName) : Descriptor(typeName) {}
};

class TSL_EXPORT ObjectContext {
protected:
	std::string mName;

public:
	ObjectContext(const std::string &name) : mName(name) {}
	const std::string & GetName() { return mName; }
};
