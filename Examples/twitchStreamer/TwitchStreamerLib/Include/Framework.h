#pragma once

#ifdef TWITCH_STREAMING_LIBRARY_DLL
#define TSL_EXPORT __declspec(dllexport)
#else
#define TSL_EXPORT __declspec(dllimport)
#endif

// To avoid including windows.h everywhere
#ifndef _WINDEF_
	#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
	DECLARE_HANDLE(HWND);
#endif

#include "obs.h"
