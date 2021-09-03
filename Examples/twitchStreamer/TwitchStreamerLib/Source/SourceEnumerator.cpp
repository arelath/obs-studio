#include <iostream>

#include "obs.h"
#include "SourceEnumerator.h"

#include "Contexts/ObsObjectDescriptors.h"
#include "Contexts/ObsSource.h"
#include "Contexts/ObsDisplay.h"

using namespace std;

SourceEnumerator::SourceEnumerator::SourceEnumerator()
{
}

void SourceEnumerator::EnumerateSources()
{
	const char *id;
	int index = 0;
	while (obs_enum_source_types(index++, &id))
	{
		cout << "Found source  type" << id << endl;

	}

	index = 0;
	while (obs_enum_input_types(index++, &id)) {
		cout << "Found Input " << id << endl;

		this->SourceFactoryMap.emplace(string(id), SourceFactory(SourceDescriptor(id)));
	}

	index = 0;
	while (obs_enum_filter_types(index++, &id)) {
		cout << "Found Filter " << id << endl;

		this->FilterFactoryMap.emplace(string(id), FilterFactory(FilterDescriptor(id)));
	}
}

