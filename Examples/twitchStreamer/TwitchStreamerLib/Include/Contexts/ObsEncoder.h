#pragma once

#include "Framework.h"
#include "ObsObjectContext.h"

MAKE_SHARED_CLASS(EncoderContext);
MAKE_SHARED_CLASS(EncoderFactory);
MAKE_SHARED_CLASS(VideoEncoderContext);
MAKE_SHARED_CLASS(AudioEncoderContext);
MAKE_SHARED_CLASS(AudioEncoderFactory);
MAKE_SHARED_CLASS(VideoEncoderFactory);

class TSL_EXPORT VideoEncoderContext : public ObjectContext {
	obs_encoder_t *encoder;

public:
	inline VideoEncoderContext(const std::string &id, const std::string & name)
		: ObjectContext(name)
	{
		encoder = obs_video_encoder_create(id.c_str(), name.c_str(), nullptr, nullptr);
	}

	inline ~VideoEncoderContext() { obs_encoder_release(encoder); }
	inline operator obs_encoder_t *() { return encoder; }
};

class TSL_EXPORT AudioEncoderContext : public ObjectContext {
	obs_encoder_t *encoder;

public:
	inline AudioEncoderContext(const std::string &id, const std::string & name)
		: ObjectContext(name)
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
