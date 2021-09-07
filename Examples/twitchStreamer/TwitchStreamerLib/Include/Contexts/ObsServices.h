#pragma once

#include "Framework.h"
#include "ObsSource.h"
#include "ObsDescriptors.h"

MAKE_SHARED_CLASS(ServiceFactory);
MAKE_SHARED_CLASS(ServiceContext);

class TSL_EXPORT ServiceContext : public ObjectContext {
	obs_service_t *service;

public:
	ServiceContext(obs_service_t *service, const std::string &name)
		: ObjectContext(name), service(service)
	{}
	~ServiceContext() { obs_service_release(service); }
	inline operator obs_service_t *() { return service; }
};

class TSL_EXPORT ServiceFactory : public Descriptor {
public:
	ServiceFactory(const char *mTypeName) : Descriptor(mTypeName) {}

	virtual ServiceContextPtr Create(
		const std::string &name,
		obs_data_t *settings = nullptr,			
		obs_data_t *hotKeyData = nullptr)
	{
		obs_service_t *service = obs_service_create(GetTypeName(), name.c_str(), settings, hotKeyData);
		return std::make_shared<ServiceContext>(service, name);
	}
};
