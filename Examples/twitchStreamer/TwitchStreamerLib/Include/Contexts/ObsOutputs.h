#pragma once

#include <string>

#include "Framework.h"
#include "ObsSource.h"
#include "ObsEncoder.h"

MAKE_SHARED_CLASS(OutputContext);
MAKE_SHARED_CLASS(OutputFactory);

class TSL_EXPORT OutputContext : public ObjectContext {
	obs_output_t *output;

	VideoEncoderContextPtr videoEncoder;
	AudioEncoderContextPtr audioEncoder;
	ServiceContextPtr serviceContext;

public:
	inline OutputContext( const char* id, const std::string &name, obs_data_t *settings = nullptr, obs_data_t *hotkey_data = nullptr)
		: ObjectContext(name)
	{
		output = obs_output_create(id, name.c_str(), settings, hotkey_data);
	}

	inline ~OutputContext() { obs_output_release(output); }
	inline operator obs_output_t *() { return output; }

	bool SetVideoEncoderToOutput(VideoEncoderContextPtr encoder)
	{
		OBS_LOG_WARNING_IF(
			videoEncoder.get() != nullptr,
			"Video Encoder already set, replacing with new video encoder.");

		videoEncoder = encoder;
		obs_output_set_video_encoder(output, *videoEncoder);
		return true;
	}

	bool SetAudioEncoderToOutput(AudioEncoderContextPtr encoder, size_t audioIndex)
	{
		OBS_LOG_WARNING_IF(
			audioEncoder.get() != nullptr,
			"Audio Encoder already set, replacing with new audio encoder.");

		audioEncoder = encoder;

		obs_output_set_audio_encoder(output, *audioEncoder, audioIndex);
		return true;
	}

	bool SetService(ServiceContextPtr service)
	{
		OBS_LOG_WARNING_IF(
			serviceContext.get() != nullptr,
			"Audio Encoder already set, replacing with new audio encoder.");

		serviceContext = service;

		obs_output_set_service(output, *serviceContext);
		return true;
	}
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

