#pragma once

#ifdef AB_PLATFORM_WINDOWS

#ifdef AB_BUILD_DLL
	#define ABSTRACT_API __declspec(dllexport)
#else
	#define ABSTRACT_API __declspec(dllimport)
#endif

#else
	#error Platform Not Supported!
#endif

#ifdef NDEBUG
#define DEBUG_CONFIG false
#define RELEASE_CONFIG true
#else
#define DEBUG_CONFIG true
#define RELEASE_CONFIG false
#endif