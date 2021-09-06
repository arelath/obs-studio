// ObsWrapper.cpp : Defines the functions for the static library.
//
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <fstream>

#include "ObsWrapper.h"
#include "Logger.h"
#include "MonitorUtils.h"

#include "Contexts/ObsSceneItem.h"

using namespace std;

ObsWrapper::ObsWrapper() : mDisplayFactory("Default")
{
	this->mLogger = make_shared<Logger>("ObsWrapper.txt");
	vec2_set(&mWindowScale, 3.0f, 3.0f);
}

ObsWrapper &ObsWrapper::operator=(const ObsWrapper &)
{
	throw new exception("Copying ObsWrapper now allowed");
}

bool ObsWrapper::InitObs(
	uint32_t baseWidth,
	uint32_t baseHeight,
	uint32_t outputWidth,
	uint32_t outputHeight)
{
	if (!obs_startup("en-US", nullptr, nullptr))
		throw "Couldn't create OBS";

	if (!ResetVideo(baseWidth, baseHeight, outputWidth, outputHeight))
		throw "Couldn't initialize video";

	obs_audio_info oai = {};
	oai.speakers = SPEAKERS_MONO;
	oai.samples_per_sec = 44100;
	if (!obs_reset_audio(&oai))
		throw "Couldn't initialize audio";


	obs_load_all_modules();

	mObsEnumerator.EnumerateSources();

	return true;
}

bool ObsWrapper::ResetVideo(
	uint32_t baseWidth,
	uint32_t baseHeight,
	uint32_t outputWidth,
	uint32_t outputHeight)
{
	// Rescale everything when the window size changes so everything looks correct.
	vec2_set(&mWindowScale, static_cast<float>(outputWidth) / baseWidth, static_cast<float>(outputHeight) / baseHeight);
	for (auto item : this->mSceneItems) {
		item.second->SetItemScale(mWindowScale);
	}

	struct obs_video_info ovi;
	ovi.adapter = 0;
	ovi.base_width = baseWidth;
	ovi.base_height = baseHeight;
	ovi.fps_num = 30000;
	ovi.fps_den = 1001;
	ovi.graphics_module = "libobs-d3d11";
	ovi.output_format = VIDEO_FORMAT_RGBA;
	ovi.output_width = outputWidth;
	ovi.output_height = outputHeight;
	ovi.scale_type = OBS_SCALE_BILINEAR;

	OBS_RETURN_BOOL(obs_reset_video(&ovi) == 0);
}

bool ObsWrapper::ResetWindowSize(HWND hwnd)
{
	RECT rect;
	uint32_t windowWidth = 100;
	uint32_t windowHeight = 100;
	if (GetWindowRect(hwnd, &rect)) {
		windowWidth = rect.right - rect.left;
		windowHeight = rect.bottom - rect.top;
	}

	uint32_t workingWidth = 100;
	uint32_t workingHeight = 100;
	MonitorUtils::GetPrimaryMonitorWorkingSize(&workingWidth, &workingHeight);

	auto displayContextPairPtr = mDisplays.find(hwnd);
	OBS_RETURN_IF_FAILED_MSG(displayContextPairPtr != mDisplays.end(), "Cannnot find monitor to pause");

	{
		// This will pause the output so we can resize if if it's enabled and restore it to it's previous state once it's out of scope
		DisplayContext::PauseDisplayIfRequired displayPause(displayContextPairPtr->second);

		OBS_RETURN_BOOL(ResetVideo(workingWidth, workingHeight, windowWidth, windowHeight));
	}

	return true;
}

ObsWrapper::~ObsWrapper()
{
	obs_shutdown();
	blog(LOG_INFO, "Number of memory leaks: %ld", bnum_allocs());
}

bool ObsWrapper::AddToCurrentScene(SourceContextPtr source, const string & name)
{
	SceneItemContextPtr item = make_shared<SceneItemContext>(obs_scene_add(*mCurrentScene, *source), name);
	mSceneItems[name] = item;

	OBS_RETURN_BOOL(item->SetItemScale(this->mWindowScale));
}

bool ObsWrapper::CreateOutput(
	const std::string &id,	// Must exist in the enumerated output types
	const std::string &name,
	obs_data_t *data,
	obs_data_t *settings)
{
	// TODO: Error checking for identical names
	mCurrentOutputContext = mObsEnumerator.GetOutputFactoryById(id)->Create(name, data, settings);
	return true;
}

bool ObsWrapper::SetVideoEncoderOnCurrentOutput(VideoEncoderContextPtr encoder)
{
	OBS_RETURN_IF_FAILED_MSG(
		mCurrentOutputContext != nullptr,
		"Must have a current scene before adding items to it.");

	obs_output_set_video_encoder(*mCurrentOutputContext, *encoder);

	return true;
}

bool ObsWrapper::SetAudioEncoderOnCurrentOutput(AudioEncoderContextPtr encoder, size_t outputChannel)
{
	OBS_RETURN_IF_FAILED_MSG(
		mCurrentOutputContext != nullptr,
		"Must have a current scene before adding items to it.");

	obs_output_set_audio_encoder(*mCurrentOutputContext, *encoder, outputChannel);

	return true;
}

bool ObsWrapper::AddOutputWindow(HWND hwnd)
{
	DisplayContextPtr displayContext = mDisplayFactory.Create(hwnd);
	mDisplays[hwnd] = displayContext;
	OBS_RETURN_BOOL(ResetWindowSize(hwnd));
}

bool ObsWrapper::RemoveOutputWindow(HWND hwnd)
{
	auto display = mDisplays.find(hwnd);
	OBS_RETURN_IF_FAILED(display == mDisplays.end());
	OBS_RETURN_BOOL(static_cast<bool>(mDisplays.erase(hwnd)));
}

ObsWrapperPtr ObsWrapper::CreateOBSOnPrimaryMonitor(uint32_t ouputWidth, uint32_t outputHeight)
{
	uint32_t inputWidth = 100;
	uint32_t inputHeight = 100;
	MonitorUtils::GetPrimaryMonitorWorkingSize(&inputWidth, &inputHeight);

	ObsWrapperPtr wrapper = ObsWrapperPtr(new ObsWrapper());
	OBS_LOG_IF_FAILED(wrapper->InitObs(inputWidth, inputHeight, ouputWidth, outputHeight));
	return wrapper;
}

// More for testing purposes for now. Not really pratical to hardcode these values
ObsWrapperPtr ObsWrapper::CreateOBS()
{
	ObsWrapperPtr wrapper = ObsWrapperPtr(new ObsWrapper());
	OBS_LOG_IF_FAILED(wrapper->InitObs(720, 480, 720, 480));
	return wrapper;
}

bool ObsWrapper::Start(uint32_t channel, SceneContextPtr scene)
{
	
	if (scene == nullptr)
	{
		if (this->mScenes.empty() == false)
		{
			// If a scene wasn't provided, we're going to pick the first one
			scene = mScenes.begin()->second;
			blog(LOG_WARNING, "No scene was provided to ObsWrapper::Start(), but scenes exists. Choosing first scene.");
		}
		else
		{
			// If we don't have a scene, just create a default for ease.
			OBS_RETURN_IF_FAILED(AddScene("default"));
		}
		
	}

	obs_set_output_source(channel, obs_scene_get_source(*scene));

	return true;
}

bool ObsWrapper::Stop()
{
	return true;
}

bool ObsWrapper::AddScene(const string& name)
{
	// We're trying to add a scene that already exists
	//RETURN_IF_FAILED(mScenes.find(name) == mScenes.end())

	mCurrentScene = mSceneFactory.Create(name);
	mScenes[name] = mCurrentScene;

	return true;
}

bool ObsWrapper::RemoveScene(const std::string& name)
{
	// If w're deleting the current scene, we don't want to continue to use it
	if (mCurrentScene != nullptr && mCurrentScene->GetName() == name)
	{
		mCurrentScene = nullptr;
	}

	OBS_RETURN_IF_FAILED(static_cast<bool>(mScenes.erase(name)));
	return true;
}


