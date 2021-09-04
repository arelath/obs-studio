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

static const int cx = 720;
static const int cy = 480;

bool windowSizeChanged = false;

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
			    WS_OVERLAPPEDWINDOW | WS_VISIBLE, 1920 / 2 - cx / 2,
			    1080 / 2 - cy / 2, cx, cy, NULL, NULL, instance,
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

		AllocConsole();

		// TODO: Setup code here
		ObsWrapperPtr obsw = ObsWrapper::CreateOBS();
		obsw->AddScene("TwitchStreamerScene");

		// You can list out all the sources as well with the enumerators
		obsw->GetEnumerator()->EnumerateSources();
		SourceFactoryPtr sourceFactory = obsw->GetEnumerator()->GetSourceFactoryByName("monitor_capture");
		SourceContextPtr monitor = sourceFactory->Create("Primary Monitor");
		obsw->AddToCurrentScene(monitor);

		obsw->AddOutputWindow(hwnd);
		obsw->Start();

		

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (windowSizeChanged)
			{
				windowSizeChanged = false;
				obsw->ResetWindowSize(hwnd);
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
