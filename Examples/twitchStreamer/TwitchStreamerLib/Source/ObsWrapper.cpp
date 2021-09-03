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

using namespace std;

ObsWrapper::ObsWrapper()
{
	// Hacky for now - TODO: fix this
	/* std::string path = getenv("PATH");
	path = "PATH=C:/code/obs-bin2/rundir/Release/bin/64bit;" + path;

	_putenv(path.c_str());
	_putenv("OBS_PLUGINS_PATH=C:/code/obs-bin2/rundir/Release/obs-plugins/64bit");
	_putenv("OBS_PLUGINS_DATA_PATH=C:/code/obs-bin2/rundir/Release/data");*/

	this->mLogger = make_shared<Logger>("ObsWrapper.txt");
}

ObsWrapper &ObsWrapper::operator=(const ObsWrapper &)
{
	throw new exception("Copying ObsWrapper now allowed");
}

shared_ptr<ObsWrapper> ObsWrapper::CreateOBS(
	uint32_t baseWidth,
	uint32_t baseHeight,
	uint32_t outputWidth,
	uint32_t outputHeight)
{
	auto wrapper = shared_ptr<ObsWrapper>(new ObsWrapper());

	if (!obs_startup("en-US", nullptr, nullptr))
		throw "Couldn't create OBS";

	struct obs_video_info ovi;
	ovi.adapter = 0;
	ovi.base_width = baseWidth;
	ovi.base_height = baseHeight;
	ovi.fps_num = 30000;
	ovi.fps_den = 1001;
	ovi.graphics_module = "libobs-opengl";
	ovi.output_format = VIDEO_FORMAT_RGBA;
	ovi.output_width = outputWidth;
	ovi.output_height = outputHeight;

	if (obs_reset_video(&ovi) != 0)
		throw "Couldn't initialize video";

	obs_load_all_modules();

	wrapper->mSourceEnumerator = make_shared<SourceEnumerator>();
	wrapper->mSourceEnumerator->EnumerateSources();

	return wrapper;
}

ObsWrapper::~ObsWrapper()
{
	obs_shutdown();
	blog(LOG_INFO, "Number of memory leaks: %ld", bnum_allocs());
}

std::shared_ptr<ObsWrapper> ObsWrapper::CreateOBSFromWindow(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	return ObsWrapper::CreateOBS(rc.right, rc.bottom, rc.right, rc.bottom);
}

// More for testing purposes for now. Mabye I'll use it once we figure out the API more.
std::shared_ptr<ObsWrapper> ObsWrapper::CreateOBS()
{
	return ObsWrapper::CreateOBS(720, 480, 720, 480);
}

bool ObsWrapper::Start()
{
	return false;
}
