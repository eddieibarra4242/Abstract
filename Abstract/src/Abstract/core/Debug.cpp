#include "Debug.h"

namespace Abstract {

	Debug::Priority Debug::state = Debug::Priority::NONE;
	bool Debug::showInfo = false;

	void Debug::__Log(const std::string& category, const Priority& p, const std::string& file, const int& line, char const* const msg, ...)
	{
		if (p > state)
		{
			return;
		}

		FILE* Stream = stdout;
		std::string level = "None";

		switch (p)
		{
		case Priority::NONE: level = "None"; break;
		case Priority::ERROR: Stream = stderr; level = "Error"; break;
		case Priority::PROFILE: level = "Profile"; break;
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