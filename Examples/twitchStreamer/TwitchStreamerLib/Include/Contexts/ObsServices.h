#pragma once

#include "Framework.h"
#include "ObsSource.h"

MAKE_SHARED_CLASS(SericeFactory);

/* class TSL_EXPORT SericeFactory : public SourceFactory {

public:
	ServiceFactory(const SourceDescriptor &sourceDescriptor)
		: SourceFactory(sourceDescriptor)
	{
	}

	virtual SourceContextPtr Create(const std::string &name)
	{
		// TODO: Will this be different? If not, remove it
		obs_source_t *source =
			obs_source_create(mSourceDescriptor.GetTypeName(),
					  name.c_str(), NULL, NULL);
		return std::make_shared<SourceContext>(source, name);
	}
};*/
