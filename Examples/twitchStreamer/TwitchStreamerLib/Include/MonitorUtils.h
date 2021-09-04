#pragma once

#include "framework.h"

// NOTE: leaving out multi-monitor support because it would add a couple hours of work. Usually this is done my enumerating all the
// displays and getting the info from them. But virtual displays are also counted (ie mirrorcast)
class MonitorUtils {
public:
	static int TSL_EXPORT GetNumOfMonitors();

	// This is the entire size including areas outside the client area
	static void TSL_EXPORT GetPrimaryMonitorSize(uint32_t &width, uint32_t &height);

	// This is just the client area of the windows shell - ie what a window would maximize to.
	static void TSL_EXPORT GetPrimaryMonitorWorkingSize(uint32_t &width, uint32_t &height);
};
