#include "Debug.h"
#include <stdexcept>

namespace Abstract {

	Debug::Priority Debug::state = Debug::Priority::Error;
	bool Debug::showInfo = false;

	void Debug::log(const std::string& category, const Priority& p, const std::string& file, const int& line, char const* const msg, ...)
	{
		if (p > state)
		{
			return;
		}

		FILE* Stream = stdout;
		std::string level = "None";

		switch (p)
		{
#ifndef NDEBUG
		case Priority::None: level = "None"; break;
#endif
		case Priority::Error: Stream = stderr; level = "Error"; break;
		case Priority::Warning: Stream = stderr; level = "Warning"; break;
		case Priority::Verbose: level = "Verbose"; break;
		case Priority::Profile: level = "Profile"; break;
		}

		if (Debug::showInfo)
		{
			fprintf(Stream, "[%s] ", category.c_str());
			fprintf(Stream, "L[%s] (%s:%d):\n", level.c_str(), file.c_str(), line);
		}

		va_list argList;
		__crt_va_start(argList, msg);
		vfprintf(Stream, msg, argList);
		__crt_va_end(argList);
		fprintf(Stream, "\n");

		if (p == Priority::Error)
		{
			throw std::runtime_error(msg);
		}
	}

	void Debug::setPriority(Priority p)
	{
		state = p;
	}

	void Debug::showAdditionalInfo(bool showInfo)
	{
		Debug::showInfo = showInfo;
	}
}