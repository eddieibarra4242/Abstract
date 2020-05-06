#pragma once
#include "Platform.h"
#include <chrono>

typedef double Delta;

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<Clock> Time;

namespace Abstract
{
	inline Delta ABSTRACT_API toSeconds(std::chrono::nanoseconds timeDiff)
	{
		return std::chrono::duration_cast<std::chrono::duration<Delta, std::milli>>(timeDiff).count() / (Delta)(1000.0);
	}
};