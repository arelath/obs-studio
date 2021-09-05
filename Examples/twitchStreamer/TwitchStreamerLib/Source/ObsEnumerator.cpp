#include <iostream>

#include "obs.h"
#include "ObsEnumerator.h"

#include "Contexts/ObsObjectDescriptors.h"
#include "Contexts/ObsAudio.h"
#include "Contexts/ObsSource.h"
#include "Contexts/ObsDisplay.h"
#include "Contexts/ObsEncoder.h"

using namespace std;

void ObsEnumerator::EnumerateSources()
{
	const char *id;
	int index = 0;
	while (obs_enum_source_types(index++, &id))
	{
		cout << "Found source type " << id << endl;

	}

	index = 0;
	while (obs_enum_input_types(index++, &id)) {
		cout << "Found Input " << id << endl;

		SourceFactoryMap.emplace(
			string(id),
			make_shared<SourceFactory>(SourceDescriptor(id)));
	}

	index = 0;
	while (obs_enum_filter_types(index++, &id)) {
		cout << "Found Filter " << id << endl;

		this->FilterFactoryMap.emplace(
			string(id),
			make_shared <FilterFactory>(SourceDescriptor(id)));
	}

	index = 0;
	while (obs_enum_service_types(index++, &id)) {
		cout << "Found Service " << id << endl;

		//this->FilterFactoryMap.emplace(
		//	string(id),
		//	make_shared<ServiceFactory>(SourceDescriptor(id)));
	}

	index = 0;
	while (obs_enum_encoder_types(index++, &id)) {
		cout << "Found Encoder " << id << endl;

		EncoderDescriptor encoderDescriptor = EncoderDescriptor(id);
		if (encoderDescriptor.GetEncoderType() == obs_encoder_type::OBS_ENCODER_AUDIO)
		{
			AudioEncoderFactoryMap[encoderDescriptor.GetTypeName()] =
				make_shared<AudioEncoderFactory>(id);
		}
		else
		{
			VideoEncoderFactoryMap[encoderDescriptor.GetTypeName()] =
				make_shared<VideoEncoderFactory>(id);
		}

		//this->FilterFactoryMap.emplace(
		//	string(id),
		//	make_shared<ServiceFactory>(SourceDescriptor(id)));
	}
}

SourceFactoryPtr ObsEnumerator::GetSourceFactoryByName(const std::string name)
{
	return SourceFactoryMap[name];
}

