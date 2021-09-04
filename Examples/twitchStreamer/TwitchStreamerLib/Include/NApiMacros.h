#pragma once

#include "Logger.h"

#ifdef USING_OBS_LOGGING
	#define LOG_PROGRAM_PLACE() " __FILE__" + ":"  +  std::to_string(__LINE__).c_str()
	#define LOG_ERROR() { blog(LOG_ERROR, LOG_PROGRAM_PLACE() ))}
	#define LOG_ERROR_MES(msg) { blog(LOG_ERROR, "%s %s", LOG_PROGRAM_PLACE(), msg) }
	#define LOG_ERROR_IF(x, msg) { blog(LOG_INFO, msg) }
#else
	#define LOG_INFO(msg) { Logger::Instance()->Log("Info: " + (msg) ); }
	#define LOG_ERROR() { Logger::Instance()->Log( "Error: " __FILE__, std::to_string(__LINE__).c_str() ); }
	#define LOG_ERROR_MSG(msg) { Logger::Instance()->Log( "Error: " __FILE__, std::to_string(__LINE__).c_str(), msg ); }
	#define LOG_ERROR_IF(x, msg) { if(x) { LOG_ERROR_MSG(msg); } }
#endif



#define RETURN_IF_FAILED(x) { if((x) == false) { LOG_ERROR(); return false; } }

#ifdef USING_NAPI

#define NAPI_RETURN_FALSE { return Napi::Boolean::New(env, false); } 
#define NAPI_RETURN_TRUE { return Napi::Boolean::New(env, true); } 

#define NAPI_RETURN_IF_FALSE(x) if ((x) == false) NAPI_RETURN_FALSE
#define NAPI_RETURN_IF_TRUE(x) if ((x) == true) NAPI_RETURN_TRUE

#define NAPI_RETURN_BOOL(x) if(x) { NAPI_RETURN_TRUE } else { NAPI_RETURN_FALSE }

#define NAPI_RETURN_IF_FAILED_MSG(x, msg) { if((x) == false) { Logger::Instance()->Log( __FILE__, std::to_string(__LINE__).c_str(), msg ); NAPI_RETURN_FALSE; } }
#define NAPI_RETURN_IF_FAILED(x) { if((x) == false) { LOG_ERROR(); NAPI_RETURN_FALSE; } }



#define NAPI_FUNC_START() Napi::Env env = info.Env();

#endif //  USING_NAPI
