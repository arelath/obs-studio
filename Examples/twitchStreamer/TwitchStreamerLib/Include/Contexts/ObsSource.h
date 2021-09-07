#pragma once

#include "Framework.h"
#include "ObsObjectContext.h"

MAKE_SHARED_CLASS(SourceContext);
MAKE_SHARED_CLASS(SourceFactory);

class TSL_EXPORT SourceContext : public ObjectContext {
	obs_source_t *source;

public:
	inline SourceContext(obs_source_t *source, const std::string & name) : ObjectContext(name), source(source) {}
	inline ~SourceContext() { obs_source_release(source); }
	inline operator obs_source_t *() { return source; }
};


class TSL_EXPORT SourceFactory {
protected:
	SourceDescriptor mSourceDescriptor;

public:
	SourceFactory(const char * id) : mSourceDescriptor(id) {}

	virtual SourceContextPtr Create(const std::string & name)
	{
		obs_source_t *source = obs_source_create(mSourceDescriptor.GetTypeName(), name.c_str(), NULL, NULL);
		return std::make_shared<SourceContext>(source, name);
	}
};
