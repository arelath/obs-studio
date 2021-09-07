#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef TWITCH_STREAMING_LIBRARY_DLL
#define TSL_EXPORT __declspec(dllexport)
#else
#define TSL_EXPORT __declspec(dllimport)
#endif

#pragma warning(disable:4251)

// To avoid including windows.h everywhere
#ifndef _WINDEF_
	#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
	DECLARE_HANDLE(HWND);
#endif

#include "obs.h"
#include <memory>
#include <string>

// This creates a shared_ptr<type> with the name typePtr
#define MAKE_SHARED_CLASS(type) class type; typedef std::shared_ptr<type> type##Ptr

// Some help on error handling
#define OBS_GET_PROGRAM_PLACE_STRING() (__FILE__ + std::string(":") + std::to_string(__LINE__))

#ifdef USING_CUSTOM_LOGGING
	#define OBS_LOG_INFO(msg) { Logger::Instance()->Log("Info: " + (msg) ); }
	#define OBS_LOG_ERROR_NO_MSG() { Logger::Instance()->Log( "Error: " __FILE__, std::to_string(__LINE__).c_str() ); }
	#define OBS_LOG_ERROR_MSG(msg) { Logger::Instance()->Log( "Error: " __FILE__, std::to_string(__LINE__).c_str(), msg ); }
	#define OBS_LOG_ERROR_IF(x, msg) { if(x) { LOG_ERROR_MSG(msg); } }
	#define OBS_LOG_WARNING(msg) { LOG_WARNING_MSG(msg); }
	#define OBS_LOG_INFO(msg) { LOG_WARNING_MSG(msg); }
#else
	#define OBS_LOG_ERROR_NO_MSG() { blog(LOG_ERROR, OBS_GET_PROGRAM_PLACE_STRING().c_str()); }
	#define OBS_LOG_ERROR_MSG(msg) { blog(LOG_ERROR, "%s %s", msg, OBS_GET_PROGRAM_PLACE_STRING().c_str()); }
	#define OBS_LOG_ERROR_IF(x, msg) { blog(LOG_ERROR, "%s %s", msg, OBS_GET_PROGRAM_PLACE_STRING().c_str()); }
	#define OBS_LOG_WARNING_IF(x, msg) { blog(LOG_WARNING, "%s %s", msg, OBS_GET_PROGRAM_PLACE_STRING().c_str()); }
	#define OBS_LOG_WARNING(msg) { blog(LOG_WARNING, "%s %s", msg, OBS_GET_PROGRAM_PLACE_STRING().c_str()); }
	#define OBS_LOG_INFO(msg) { blog(LOG_INFO, "%s %s", msg, OBS_GET_PROGRAM_PLACE_STRING().c_str()); }
#endif

// These macros let us do extra things when we do common function or make failures easier to log.
// Also because their macros, changing their behavoir affects the entire library (ie throwing on failures).
#define OBS_RETURN_IF_FAILED(x) { if((x) == false) { OBS_LOG_ERROR_NO_MSG(); return false; } }
#define OBS_RETURN_IF_FAILED_MSG(x, msg) { if((x) == false) { OBS_LOG_ERROR_MSG(msg); return false; } }
#define OBS_RETURN_BOOL(x) { OBS_RETURN_IF_FAILED(x); return true; }
#define OBS_RETURN(x) { return (x); }
#define OBS_LOG_IF_FAILED(x) { if ((x) == false) OBS_LOG_ERROR_NO_MSG(); }
#define OBS_LOG_IF_FAILED_MSG(x, msg) { if ((x) == false) OBS_LOG_ERROR_MSG(msg); }

#ifdef USING_NAPI

#define NAPI_RETURN_FALSE { return Napi::Boolean::New(env, false); } 
#define NAPI_RETURN_TRUE { return Napi::Boolean::New(env, true); } 

#define NAPI_RETURN_IF_FALSE(x) if ((x) == false) NAPI_RETURN_FALSE
#define NAPI_RETURN_IF_TRUE(x) if ((x) == true) NAPI_RETURN_TRUE

#define NAPI_RETURN_BOOL(x) if(x) { NAPI_RETURN_TRUE } else { NAPI_RETURN_FALSE }

#define NAPI_RETURN_IF_FAILED_MSG(x, msg) { if((x) == false) { Logger::Instance()->Log( __FILE__, std::to_string(__LINE__).c_str(), msg ); NAPI_RETURN_FALSE; } }
#define NAPI_RETURN_IF_FAILED(x) { if((x) == false) { LOG_ERROR_NO_MSG(); NAPI_RETURN_FALSE; } }


#define NAPI_FUNC_START() Napi::Env env = info.Env();

#endif //  USING_NAPI
