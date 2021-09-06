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
	obs_data_t *settings;

public:
	EncoderContext(const std::string& name)
		: ObjectContext(name)
	{
		settings = obs_data_create();
	}

	~EncoderContext()
	{
		obs_data_release(settings);
	}

	// NOTE:
	// Looks like most encoders come with resonable defaults, so these may not be needed based on your scenario

	bool SetBitrate(int bitRate)
	{
		// TODO: Check if property is in range and has been set
		obs_data_set_int(settings, "BitRate", bitRate);
		return true;
	}

	bool SetRateControl(const char* rateControl = "CBR")
	{
		obs_data_set_string(settings, "rate_control", rateControl);
		return true;
	}

	bool SetKeyRateSec(int keyRate = 2)
	{
		// Documentation says this is correct, but a design mistake
		obs_data_set_int(settings, "keyint_sec", keyRate);
		return true;
	}
};

class TSL_EXPORT VideoEncoderContext : public EncoderContext
{
public:
	inline VideoEncoderContext(const std::string &id, const std::string & name)
		: EncoderContext(name)
	{
		encoder = obs_video_encoder_create(id.c_str(), name.c_str(), nullptr, nullptr);
	}

	inline ~VideoEncoderContext() { obs_encoder_release(encoder); }
	inline operator obs_encoder_t *() { return encoder; }
};

class TSL_EXPORT AudioEncoderContext : public EncoderContext {
	obs_encoder_t *encoder;
	

public:
	inline AudioEncoderContext(const std::string &id, const std::string & name)
		: EncoderContext(name)
	{
		encoder = obs_audio_encoder_create(id.c_str(), name.c_str(), nullptr, 0, nullptr);
	}

	inline ~AudioEncoderContext() { obs_encoder_release(encoder); }
	inline operator obs_encoder_t *() { return encoder; }
};

class TSL_EXPORT VideoEncoderFactory {
protected:
	EncoderDescriptor mEncoderDescriptor;

public:
	VideoEncoderFactory(const char *id)
		: mEncoderDescriptor(id)
	{
	}

	virtual VideoEncoderContextPtr Create(const std::string &name)
	{
		VideoEncoderContextPtr encoder = std::make_shared<VideoEncoderContext>(mEncoderDescriptor.GetTypeName(), name); 
		return encoder;
	}
};

class TSL_EXPORT AudioEncoderFactory {
protected:
	EncoderDescriptor mEncoderDescriptor;

public:
	AudioEncoderFactory(const char *id) : mEncoderDescriptor(id) {}

	virtual AudioEncoderContextPtr Create (const std::string &name)
	{
		AudioEncoderContextPtr encoder = std::make_shared<AudioEncoderContext>(mEncoderDescriptor.GetTypeName(), name);
		return encoder;
	}
};
