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
#ifdef USING_CUSTOM_LOGGING
	#define LOG_INFO(msg) { Logger::Instance()->Log("Info: " + (msg) ); }
	#define LOG_ERROR_NO_MSG() { Logger::Instance()->Log( "Error: " __FILE__, std::to_string(__LINE__).c_str() ); }
	#define LOG_ERROR_MSG(msg) { Logger::Instance()->Log( "Error: " __FILE__, std::to_string(__LINE__).c_str(), msg ); }
	#define LOG_ERROR_IF(x, msg) { if(x) { LOG_ERROR_MSG(msg); } }
#else
	#define GET_PROGRAM_PLACE_STRING()  (__FILE__ + std::string(":")  +  std::to_string(__LINE__))
	#define LOG_ERROR_NO_MSG() { blog(LOG_ERROR, GET_PROGRAM_PLACE_STRING().c_str()); }
	#define LOG_ERROR_MES(msg) { blog(LOG_ERROR, "%s %s", GET_PROGRAM_PLACE_STRING().c_str(), msg); }
	#define LOG_ERROR_IF(x, msg) { blog(LOG_INFO, msg); }
#endif


#define RETURN_IF_FAILED(x) { if((x) == false) { LOG_ERROR_NO_MSG(); return false; } }
#define RETURN_BOOL(x) { RETURN_IF_FAILED(x); return true; }
#define LOG_IF_FAILED(x) { if ((x) == false) LOG_ERROR_NO_MSG(); }

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
