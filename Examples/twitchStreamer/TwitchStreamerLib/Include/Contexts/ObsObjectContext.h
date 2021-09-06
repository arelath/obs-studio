#pragma once

#include "Framework.h"
#include "obs.h"
#include "ObsDescriptors.h"

#include <string>

MAKE_SHARED_CLASS(ObjectContext);


class TSL_EXPORT ObjectContext {
protected:
	std::string mName;

public:
	ObjectContext(const std::string &name) : mName(name) {}
	const std::string & GetName() { return mName; }
};
