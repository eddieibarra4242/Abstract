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