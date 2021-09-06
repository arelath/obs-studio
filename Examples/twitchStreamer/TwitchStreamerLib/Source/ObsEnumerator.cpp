#include <iostream>

#include "obs.h"
#include "ObsEnumerator.h"

#include "Contexts/ObsObjectDescriptors.h"
#include "Contexts/ObsAudio.h"
#include "Contexts/ObsSource.h"
#include "Contexts/ObsDisplay.h"
#include "Contexts/ObsEncoder.h"
#include "Contexts/ObsOutputs.h"

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
		EncoderDescriptor encoderDescriptor = EncoderDescriptor(id);
		if (encoderDescriptor.GetEncoderType() == obs_encoder_type::OBS_ENCODER_AUDIO)
		{
			cout << "Found Audio Encoder " << id << endl;
			AudioEncoderFactoryMap[encoderDescriptor.GetTypeName()] =
				make_shared<AudioEncoderFactory>(id);
		} else if (encoderDescriptor.GetEncoderType() == obs_encoder_type::OBS_ENCODER_VIDEO)
		{
			cout << "Found Video Encoder " << id << endl;
			VideoEncoderFactoryMap[encoderDescriptor.GetTypeName()] =
				make_shared<VideoEncoderFactory>(id);
		}
		else
		{
			blog(LOG_WARNING,
			     "Unsupported uncoder type for encoder %s",
			     encoderDescriptor.GetTypeName());
		}
		}

	index = 0;
	while (obs_enum_output_types(index++, &id))
	{
		cout << "Found Output Type " << id << endl;

		OutputFactoryMap.emplace(id, make_shared<OutputFactory>(id));
	}

	index = 0;
	while (obs_enum_service_types(index++, &id)) {
		cout << "Found Service Type " << id << endl;

		//this->FilterFactoryMap.emplace(
		//	string(id),
		//	make_shared<ServiceFactory>(SourceDescriptor(id)));
	}
}

SourceFactoryPtr ObsEnumerator::GetSourceFactoryById(const std::string& id)
{
	return SourceFactoryMap[id];
}

FilterFactoryPtr ObsEnumerator::GetFilterFactoryById(const std::string &id)
{
	return FilterFactoryMap[id];
}

ServiceFactoryPtr ObsEnumerator::GetServicesFactoryById(const std::string &id)
{
	return ServiceFactoryMap[id];
}


VideoEncoderFactoryPtr ObsEnumerator::GetVideoEncoderFactoryById(const std::string &id)
{
	return VideoEncoderFactoryMap[id];
}

AudioEncoderFactoryPtr ObsEnumerator::GetAudioEncoderFactoryById(const std::string &id)
{
	return AudioEncoderFactoryMap[id];
}

OutputFactoryPtr ObsEnumerator::GetOutputFactoryById(const std::string& id)
{
	return OutputFactoryMap[id];
}

