#pragma once

#include <memory>

#include "Framework.h"
#include "ObsEnumerator.h"
#include "Contexts/ObsScene.h"
#include "Contexts/ObsDisplay.h"
#include "Contexts/ObsOutputs.h"

MAKE_SHARED_CLASS(Logger);
MAKE_SHARED_CLASS(ObsWrapper);
MAKE_SHARED_CLASS(SceneItemContext);
MAKE_SHARED_CLASS(VideoEncoderContext);

class TSL_EXPORT ObsWrapper
{
public:
	static ObsWrapperPtr CreateOBSOnPrimaryMonitor(uint32_t ouputWidth, uint32_t outputHeight);

	// Only for testing purposes - mostly hardcoded values and might be removed in a final version
	static ObsWrapperPtr CreateOBS();

	bool ObsWrapper::CreateOutput(
		const std::string &id, // Must exist in the enumerated output types
		const std::string &name,
		obs_data_t *data = nullptr,
		obs_data_t *settings = nullptr);

	// This finds all public sources, filters, inputs ect so we can list them off to the user in a UI
	ObsEnumerator* GetEnumerator() { return &mObsEnumerator; }

	bool AddToCurrentScene(SourceContextPtr source, const std::string &name);

	// Outputs do things like stream or encode vide/audio

	bool AddOutputWindow(HWND hwnd);
	bool RemoveOutputWindow(HWND hwnd);

	bool SetVideoEncoderOnCurrentOutput(VideoEncoderContextPtr videoEnocoder);
	bool SetAudioEncoderOnCurrentOutput(AudioEncoderContextPtr audioEncoder,
					    size_t outputChannel = 0);

	bool ResetWindowSize(HWND hwnd);

	bool IsReady() { return mIsReady; }

	//bool AddSourceByName(const std::string)

	bool AddScene(const std::string& name);
	bool RemoveScene(const std::string &name);
	
	bool Start(uint32_t channel = 0, SceneContextPtr scene = nullptr);
	bool Stop();

	virtual ~ObsWrapper();

private:
	ObsWrapper();

	ObsWrapper(const ObsWrapper&);                 // Prevent copy-construction
	ObsWrapper& operator=(const ObsWrapper&);      // Prevent assignment

	bool InitObs(
		uint32_t baseWidth,
		uint32_t baseHeight,
		uint32_t outputWidth,
		uint32_t outputHeight);

	bool ResetVideo(
		uint32_t baseWidth,
		uint32_t baseHeight,
		uint32_t outputWidth,
		uint32_t outputHeight);

	LoggerPtr mLogger;
	ObsEnumerator mObsEnumerator;

	// There can be more than one, this just makes it easier to set up and forget.
	SceneContextPtr mCurrentScene;

	// There can be more than one, this just makes it easier to set up and forget.
	OutputContextPtr mCurrentOutputContext;	

	std::map<std::string, SceneContextPtr> mScenes;

	// Probably won't ever need more than one, but since everything else if using a factory, we might as well
	SceneFactory mSceneFactory;

	// Again, we could do more here (VR displays or something), but for now we're keeping it simple.
	DisplayFactory mDisplayFactory;
	std::map<HWND, DisplayContextPtr> mDisplays;
	std::map<const std::string, SceneItemContextPtr> mSceneItems;

	struct vec2 mWindowScale;

	bool mIsReady = false;
};

