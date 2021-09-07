#pragma once

#include <map>

#include "Framework.h"

#include "Contexts/ObsSource.h"
#include "Contexts/ObsFilter.h"
#include "Contexts/ObsServices.h"
#include "Contexts/ObsEncoder.h"

MAKE_SHARED_CLASS(ObsEnumerator);
MAKE_SHARED_CLASS(ServiceFactory);
MAKE_SHARED_CLASS(EncoderFactory);
MAKE_SHARED_CLASS(OutputFactory);
MAKE_SHARED_CLASS(ServiceFactory);

class TSL_EXPORT ObsEnumerator {
public:
	// we may have to enumerate these again if they change
	void EnumerateSources();

	SourceFactoryPtr GetSourceFactoryById(const std::string& id);
	FilterFactoryPtr GetFilterFactoryById(const std::string &id);
	VideoEncoderFactoryPtr GetVideoEncoderFactoryById(const std::string &id);
	AudioEncoderFactoryPtr GetAudioEncoderFactoryById(const std::string &id);
	OutputFactoryPtr GetOutputFactoryById(const std::string& id);
	ServiceFactoryPtr GetServiceFactoryById(const std::string &id);

private:
	std::map<std::string, SourceFactoryPtr> SourceFactoryMap;
	std::map<std::string, FilterFactoryPtr> FilterFactoryMap;
	std::map<std::string, ServiceFactoryPtr> ServiceFactoryMap;
	std::map<std::string, VideoEncoderFactoryPtr> VideoEncoderFactoryMap;
	std::map<std::string, AudioEncoderFactoryPtr> AudioEncoderFactoryMap;
	std::map<std::string, OutputFactoryPtr> OutputFactoryMap;
};

