#pragma once

#include <string>

#include "Framework.h"
#include "ObsObjectContext.h"


class TSL_EXPORT SourceContext {
	obs_source_t *source;

public:
	inline SourceContext(obs_source_t *source) : source(source) {}
	inline ~SourceContext() { obs_source_release(source); }
	inline operator obs_source_t *() { return source; }
};


class TSL_EXPORT SourceFactory {
protected:
	SourceDescriptor mSourceDescriptor;

public:
	SourceFactory(SourceDescriptor sourceDescriptor) : mSourceDescriptor(sourceDescriptor)
	{
	}

	virtual SourceContext Create(std::string name)
	{
		return SourceContext(obs_source_create(mSourceDescriptor.GetTypeName(), name.c_str(), NULL, NULL));
	}
};
