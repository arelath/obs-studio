#pragma once

#include <memory>

#include "Framework.h"
#include "ObsEnumerator.h"
#include "Contexts/ObsScene.h"
#include "Contexts/ObsDisplay.h"

MAKE_SHARED_CLASS(Logger);
MAKE_SHARED_CLASS(ObsWrapper);
MAKE_SHARED_CLASS(SceneItemContext);

class TSL_EXPORT ObsWrapper
{
public:
	static ObsWrapperPtr CreateOBSOnPrimaryMonitor(uint32_t ouputWidth, uint32_t outputHeight);

	// Only for testing purposes - mostly hardcoded values and might be removed in a final version
	static ObsWrapperPtr CreateOBS();

	// This finds all public sources, filters, inputs ect so we can list them off to the user in a UI
	ObsEnumerator* GetEnumerator() { return &mObsEnumerator; }

	bool AddToCurrentScene(SourceContextPtr source, const std::string &name);

	bool AddOutputWindow(HWND hwnd);
	bool RemoveOutputWindow(HWND hwnd);

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

	bool ResetVideo(uint32_t baseWidth, uint32_t baseHeight,
			uint32_t outputWidth, uint32_t outputHeight);

	LoggerPtr mLogger;
	ObsEnumerator mObsEnumerator;

	SceneContextPtr mCurrentScene;
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

