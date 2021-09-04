#pragma once

#include <map>
#include <string>
#include <memory>

#include "Framework.h"
#include "obs.h"
#include "Contexts/ObsObjectDescriptors.h"
#include "Contexts/ObsSource.h"
#include "Contexts/ObsFilter.h"

class SourceDescriptor;
class SourceContext;

class FilterDescriptor;
class FilterContext;

class DisplayFactory;

MAKE_SHARED_CLASS(SourceEnumerator);

class SourceEnumerator {
public:
	SourceEnumerator();

	// we may have to enumerate these again if they change
	void EnumerateSources();

	SourceFactoryPtr GetSourceFactoryByName(const std::string name);

private:
	std::map<std::string, SourceFactoryPtr> SourceFactoryMap;
	std::map<std::string, FilterFactoryPtr> FilterFactoryMap;

};

