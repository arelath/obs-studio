#pragma once

#include <string>

#include "Framework.h"
#include "ObsSource.h"

class TSL_EXPORT FilterFactory : public SourceFactory {

public:
	FilterFactory(SourceDescriptor sourceDescriptor)
		: SourceFactory(sourceDescriptor)
	{
	}

	SourceContext Create(std::string name)
	{
		return SourceContext(
			obs_source_create(mSourceDescriptor.GetTypeName(),
					  name.c_str(), NULL, NULL));
	}
};
