#pragma once

#include "Framework.h"
#include "ObsObjectContext.h"

MAKE_SHARED_CLASS(EncoderContext);
MAKE_SHARED_CLASS(EncoderFactory);
MAKE_SHARED_CLASS(VideoEncoderContext);
MAKE_SHARED_CLASS(AudioEncoderContext);
MAKE_SHARED_CLASS(AudioEncoderFactory);
MAKE_SHARED_CLASS(VideoEncoderFactory);

class TSL_EXPORT EncoderContext : public ObjectContext {
protected:
	obs_encoder_t *encoder;

	inline obs_data_t* GetSettings()
	{
		return obs_encoder_get_settings(encoder);
	}

public:
	EncoderContext(const std::string& name)
		: ObjectContext(name)
	{
	}

	virtual ~EncoderContext() { obs_encoder_release(encoder); }
	inline operator obs_encoder_t *() { return encoder; }

	// NOTE:
	// Looks like most encoders come with resonable defaults, so these may not be needed based on your scenario

	bool SetBitrate(int bitRate)
	{
		// TODO: Check if property is in range and has been set
		obs_data_set_int(GetSettings(), "bitrate", bitRate);
		return true;
	}

	bool SetRateControl(const char* rateControl = "cbr")
	{
		obs_data_set_string(GetSettings(), "rate_control", rateControl);
		return true;
	}

	bool SetKeyRateSec(int keyRate = 2)
	{
		// Documentation says this is correct, but a design mistake
		obs_data_set_int(GetSettings(), "keyint_sec", keyRate);
		return true;
	}
};

class TSL_EXPORT VideoEncoderContext : public EncoderContext
{
public:
	inline VideoEncoderContext(const std::string &id, const std::string & name, obs_data_t* settings)
		: EncoderContext(name)
	{
		encoder = obs_video_encoder_create(id.c_str(), name.c_str(), settings, nullptr);
	}
	inline operator obs_encoder_t *() { return encoder; }

	void SetVideo(video_t* video)
	{
		obs_encoder_set_video(encoder, video);
	}
};

class TSL_EXPORT AudioEncoderContext : public EncoderContext {
public:
	inline AudioEncoderContext(const std::string &id, const std::string & name, obs_data_t* settings)
		: EncoderContext(name)
	{
		encoder = obs_audio_encoder_create(id.c_str(), name.c_str(), settings, 0, nullptr);
	}

	void SetAudio(audio_t *audio)
	{
		obs_encoder_set_audio(encoder, audio);
	}
	inline operator obs_encoder_t *() { return encoder; }
};

class TSL_EXPORT VideoEncoderFactory {
protected:
	EncoderDescriptor mEncoderDescriptor;

public:
	VideoEncoderFactory(const char *id)
		: mEncoderDescriptor(id)
	{}

	virtual VideoEncoderContextPtr Create(const std::string &name, obs_data_t* settings)
	{
		VideoEncoderContextPtr encoder =
			std::make_shared<VideoEncoderContext>(
				mEncoderDescriptor.GetTypeName(),
				name,
				settings); 
		return encoder;
	}
};

class TSL_EXPORT AudioEncoderFactory {
protected:
	EncoderDescriptor mEncoderDescriptor;

public:
	AudioEncoderFactory(const char *id) : mEncoderDescriptor(id) {}

	virtual AudioEncoderContextPtr Create (const std::string &name, obs_data_t* settings)
	{
		AudioEncoderContextPtr encoder =
			std::make_shared<AudioEncoderContext>(
				mEncoderDescriptor.GetTypeName(),
				name,
				settings);
		return encoder;
	}
};
