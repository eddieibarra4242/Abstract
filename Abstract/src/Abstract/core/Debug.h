#pragma once
#include "Platform.h"
#include <string>
#include <stdio.h>

#define DEBUG_LOG(category, level, message, ...) Debug::log(category, level, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define DEBUG_WARN(category, message, ...) Debug::log(category, Debug::Priority::Warning, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define DEBUG_ERROR(category, message, ...) Debug::log(category, Debug::Priority::Error, __FILE__, __LINE__, message, ##__VA_ARGS__)

namespace Abstract {

	struct ABSTRACT_API Debug
	{
	public:
		enum class Priority
		{
#ifndef NDEBUG
			//This should never be used in a release build, this is purely for quick debug messages
			None,
#endif
			Error,
			Warning,
			ToFile, //not implemented yet
			Verbose,
			Profile
		};

		static void log(const std::string& category, const Priority& p, const std::string& file, const int& line, char const* const msg, ...);
		static void toFile(const std::string& category, const Priority& p, const std::string& file, const int& line, char const* const msg, ...); //TBI

		static Priority state;
		static bool showInfo;

		static void setPriority(Priority p);
		static void showAdditionalInfo(bool showInfo);
	};

}