#pragma once
#include "Platform.h"
#include <string>
#include <stdio.h>

#define DEBUG_LOG(category, level, message, ...) Abstract::Debug::__Log(category, level, __FILE__, __LINE__, message, ##__VA_ARGS__)

namespace Abstract {

	struct ABSTRACT_API Debug
	{
	public:
		enum class Priority
		{
			NONE,
			ERROR,
			TOFILE, //not implemented yet
			PROFILE
		};

		static Priority state;
		static bool showInfo;

		static void __Log(const std::string& category, const Priority& p, const std::string& file, const int& line, char const* const msg, ...);

		static void setPriority(Priority p);
		static void showAdditionalInfo(bool showInfo);
	};

}