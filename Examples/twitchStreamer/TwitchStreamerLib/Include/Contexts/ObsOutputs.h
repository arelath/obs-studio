#pragma once

#include <string>

#include "Framework.h"
#include "ObsSource.h"
#include "ObsObjectDescriptors.h"

MAKE_SHARED_CLASS(OutputContext);
MAKE_SHARED_CLASS(OutputFactory);

class TSL_EXPORT OutputContext : public ObjectContext {
	obs_output_t *output;

public:
	inline OutputContext( const char* id, const std::string &name, obs_data_t *settings = nullptr, obs_data_t *hotkey_data = nullptr)
		: ObjectContext(name)
	{
		obs_output_create(id, name.c_str(), settings, hotkey_data);
	}

	inline ~OutputContext() { obs_output_release(output); }
	inline operator obs_output_t *() { return output; }
};


class TSL_EXPORT OutputFactory : public Descriptor {
public:
	OutputFactory(const char* typeName) : Descriptor(typeName) {}

	virtual OutputContextPtr Create(const std::string &name,
				     obs_data_t *settings = nullptr,
				     obs_data_t *hotkey_data = nullptr)
	{
		return std::make_shared<OutputContext>(GetTypeName(), name, settings, hotkey_data);
	}
};

