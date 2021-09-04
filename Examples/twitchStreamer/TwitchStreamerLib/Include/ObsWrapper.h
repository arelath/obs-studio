#pragma once

#include <memory>

#include "Framework.h"
#include "SourceEnumerator.h"
#include "Contexts/ObsScene.h"
#include "Contexts/ObsDisplay.h"

MAKE_SHARED_CLASS(Logger);
MAKE_SHARED_CLASS(ObsWrapper);

class ObsWrapper
{
public:
	virtual ~ObsWrapper();

	static ObsWrapperPtr CreateOBS();

	SourceEnumerator* GetEnumerator() { return &mSourceEnumerator; }

	bool AddToCurrentScene(SourceContextPtr source);

	bool AddOutputWindow(HWND hwnd);
	bool RemoveOutputWindow(HWND hwnd);

	bool ResetWindowSize(HWND hwnd);

	bool IsReady() { return mIsReady; }

	//bool AddSourceByName(const std::string)

	bool AddScene(const std::string& name);
	bool RemoveScene(const std::string &name);
	
	bool Start(uint32_t channel = 0, SceneContextPtr scene = nullptr);
	bool Stop();

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
	SourceEnumerator mSourceEnumerator;

	SceneContextPtr mCurrentScene;
	std::map<std::string, SceneContextPtr> mScenes;

	// Probably won't ever need more than one, but since everything else if using a factory, we might as well
	SceneFactory mSceneFactory;

	// Again, we could do more here (VR displays or something), but for now we're keeping it simple.
	DisplayFactory mDisplayFactory;
	std::map<HWND, DisplayContextPtr> mDisplays; 

	bool mIsReady = false;
};

