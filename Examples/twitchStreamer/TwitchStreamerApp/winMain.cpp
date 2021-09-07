#include <stdio.h>
#include <time.h>
#include <windows.h>

#include <util/base.h>
#include <graphics/vec2.h>
#include <media-io/audio-resampler.h>
#include <obs.h>
#include <obs-config.h>

#include <intrin.h>

#include "ObsWrapper.h"
#include "ObsEnumerator.h"
#include "Contexts/ObsServices.h"

static const int cx = 1280;
static const int cy = 720;

bool windowSizeChanged = false;

const char *videoEncoderId = "obs_x264";
const char *audioEncoderId = "CoreAudio_AAC";
const char *serviceId = "rtmp_common";
const char *streamingOutputId = "rtmp_output";
const char *displaySourceId = "monitor_capture";

// Settings are {"settings":{"bwtest":false,"key":"live_724000879_nWZk5D5ghyn4Vt8Z9aM6Asv8Q16hG9","server":"rtmp://live-sea.twitch.tv/app","service":"Twitch"},"type":"rtmp_common"}
const char *privateKey = "live_724000879_nWZk5D5ghyn4Vt8Z9aM6Asv8Q16hG9";

static LRESULT CALLBACK sceneProc(HWND hwnd, UINT message, WPARAM wParam,
				  LPARAM lParam)
{
	switch (message) {

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:	// Position or size change
		windowSizeChanged = true;
		break;


	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

static HWND CreateTestWindow(HINSTANCE instance)
{
	WNDCLASS wc;

	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = TEXT("Streamer");
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = (WNDPROC)sceneProc;

	if (!RegisterClass(&wc))
		return 0;

	return CreateWindow(TEXT("Streamer"), TEXT("Streamer"),
			    WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100,
			    100, cx, cy, NULL, NULL, instance,
			    NULL);
}



/* --------------------------------------------------- */

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine,
		   int numCmd)
{
	HWND hwnd = NULL;
	try {
		hwnd = CreateTestWindow(instance);
		if (!hwnd)
			throw "Couldn't create main window";

		// Debugging output to see log in realtime. normal std input and output can be used in this console
		AllocConsole();
		FILE *fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);

		//WaitForDebuggerToAttach();

		// TODO: Setup code here
		ObsWrapperPtr obsw = ObsWrapper::CreateOBS();

		obs_data_t *twitchData = obs_data_create();
		obs_data_set_string(twitchData, "service", "twitch");
		obs_data_set_string(twitchData, "server", "rtmp://live-sea.twitch.tv/app");
		obs_data_set_string(twitchData, "url", "rtmp://live-sea.twitch.tv/app");
		obs_data_set_string(twitchData, "stream_key_link", "https://dashboard.twitch.tv/settings/stream" );
		obs_data_set_string(twitchData, "key", privateKey);
		obs_data_set_string(twitchData, "type", "rtmp_common");
		obs_data_set_bool(twitchData, "bwtest", false);

		// This will be set as the current output until we set it to something else and everything added to it.
		obsw->CreateOutput("rtmp_output", "rtmp_output", nullptr, twitchData);
		obsw->AddScene("TwitchStreamerScene");

		obsw->AddOutputWindow(hwnd);

		obs_data_t *videoEncoderSettings = obs_data_create();
		obs_data_set_int(videoEncoderSettings, "bitrate", 2500);
		obs_data_set_int(videoEncoderSettings, "keyrate_sec", 2);
		obs_data_set_string(videoEncoderSettings, "rate_control", "cbr");

		obs_data_t *audioEncoderSettings = obs_data_create();
		obs_data_set_int(videoEncoderSettings, "bitrate", 128);
		obs_data_set_int(videoEncoderSettings, "keyrate_sec", 2);
		obs_data_set_string(videoEncoderSettings, "rate_control", "cbr");

		auto videoEncoder =
			obsw->GetEnumerator()
				->GetVideoEncoderFactoryById("obs_x264")
				->Create("VideoEncoder", videoEncoderSettings);
		videoEncoder->SetVideo(obs_get_video());

		auto audioEncoder =
			obsw->GetEnumerator()
				->GetAudioEncoderFactoryById("CoreAudio_AAC")
				->Create("AudioEncoder", audioEncoderSettings);
		audioEncoder->SetAudio(obs_get_audio());

		/* audioEncoder->SetBitrate(128);
		audioEncoder->SetKeyRateSec(2);
		videoEncoder->SetBitrate(2500);
		videoEncoder->SetKeyRateSec(2);
		videoEncoder->SetRateControl("cbr");*/

		obsw->SetVideoEncoderOnCurrentOutput(videoEncoder);
		obsw->SetAudioEncoderOnCurrentOutput(audioEncoder, 0);

		ServiceFactoryPtr servicesFactory = obsw->GetEnumerator()->GetServiceFactoryById( "rtmp_common");
		ServiceContextPtr twitchSerive = servicesFactory->Create("TwitchStreamer", twitchData);

		// You can list out all the sources as well with the enumerators
		SourceFactoryPtr sourceFactory = obsw->GetEnumerator()->GetSourceFactoryById(displaySourceId);
		SourceContextPtr monitor = sourceFactory->Create("Primary Monitor");
		SourceContextPtr audio = sourceFactory->Create("sync_audio");
		SourceContextPtr webcam = sourceFactory->Create("dshow_input");

		obsw->AddToCurrentScene(monitor, "Primary Monitor");
		obsw->AddToCurrentScene(audio, "Primary Audio");

		if (webcam.get() != nullptr) {
			vec2 scale = {1.0 / 8.0, 1.0 / 8.0};
			vec2 position = {1.0 / 10.0, 1.0 / 10.0 };
			obsw->AddToCurrentScene(webcam, "webcam", scale, position);
		}
		

		obsw->SetOutputToCurrentScene(0, *monitor);
		obsw->SetOutputToCurrentScene(0, *audio);
		obs_output_set_service(obsw->GetCurrentOutput(), *twitchSerive);

		obsw->Start();

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (windowSizeChanged)
			{
				windowSizeChanged = false;
				//obsw->ResetWindowSize(hwnd);
			}
		}

	} catch (char *error) {
		MessageBoxA(NULL, error, NULL, 0);
	}

	blog(LOG_INFO, "Number of memory leaks: %ld", bnum_allocs());
	DestroyWindow(hwnd);

	UNUSED_PARAMETER(prevInstance);
	UNUSED_PARAMETER(cmdLine);
	UNUSED_PARAMETER(numCmd);
	return 0;
}
