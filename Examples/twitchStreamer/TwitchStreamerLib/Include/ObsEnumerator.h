#pragma once

#include <map>
#include <string>
#include <memory>

#include "Framework.h"
#include "obs.h"
#include "Contexts/ObsObjectDescriptors.h"
#include "Contexts/ObsSource.h"
#include "Contexts/ObsFilter.h"
#include "Contexts/ObsServices.h"
#include "Contexts/ObsEncoder.h"

class SourceDescriptor;
class SourceContext;

class FilterDescriptor;
class FilterContext;

class DisplayFactory;

class ServiceDescritor;
class ServiceFactory;

class EncoderDescriptor;
class EncoderFactory;

MAKE_SHARED_CLASS(ObsEnumerator);
MAKE_SHARED_CLASS(ServiceFactory);
MAKE_SHARED_CLASS(EncoderFactory);

class TSL_EXPORT ObsEnumerator {
public:
	// we may have to enumerate these again if they change
	void EnumerateSources();

	SourceFactoryPtr GetSourceFactoryByName(const std::string name);

private:
	std::map<std::string, SourceFactoryPtr> SourceFactoryMap;
	std::map<std::string, FilterFactoryPtr> FilterFactoryMap;
	std::map<std::string, ServiceFactoryPtr> ServiceFactoryMap;
	std::map<std::string, VideoEncoderFactoryPtr> VideoEncoderFactoryMap;
	std::map<std::string, AudioEncoderFactoryPtr> AudioEncoderFactoryMap;

};

