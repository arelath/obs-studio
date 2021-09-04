#include <Windows.h>

#include "MonitorUtils.h"



int TSL_EXPORT MonitorUtils::GetNumOfMonitors()
{
	return GetSystemMetrics(SM_CMONITORS);
}

void TSL_EXPORT MonitorUtils::GetPrimaryMonitorSize(uint32_t *width, uint32_t *height)
{
	(*width) = GetSystemMetrics(SM_CXSCREEN);
	(*height) = GetSystemMetrics(SM_CYSCREEN);
}

void TSL_EXPORT MonitorUtils::GetPrimaryMonitorWorkingSize(uint32_t *width, uint32_t *height)
{
	RECT workingArea;
	OBS_LOG_IF_FAILED(SystemParametersInfoA(SPI_GETWORKAREA, 0, &workingArea, 0));

	// Pretty sure this is in virtual coordinates of the entire working space
	(*width) = workingArea.right - workingArea.left;
	(*height) = workingArea.bottom - workingArea.top;
}
