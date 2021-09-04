#pragma once

#include "Framework.h"
#include "ObsObjectContext.h"

#include <string>

MAKE_SHARED_CLASS(DisplayContext);

class TSL_EXPORT DisplayContext : public ObjectContext {
	obs_display_t *display;

public:
	inline DisplayContext(HWND hWnd, uint32_t backgroundColor = 0);
	inline ~DisplayContext() { obs_display_destroy(display); }
	inline operator obs_display_t *() { return display; }

	bool PauseOutput()
	{
		OBS_LOG_IF_FAILED_MSG(obs_display_enabled(display) == false, "Pausing display output failed because it's already paused");
		obs_display_set_enabled(display, false);
		return true;
	}

	bool ResumeOutput()
	{
		OBS_LOG_IF_FAILED_MSG(obs_display_enabled(display) == true, "Resuming display output failed because it's already running");
		obs_display_set_enabled(display, true);
		return true;
	}

	// Helper class to pause active outputs and restore them on destruction. 
	class PauseDisplayIfRequired {
		bool resumeRequired = true;
		DisplayContextPtr mDisplay;

	public:
		PauseDisplayIfRequired(DisplayContextPtr displayContext)
			: mDisplay(displayContext)
		{
			obs_display_t *display = *mDisplay;
			resumeRequired = obs_display_enabled(display);
			if (resumeRequired)
				obs_display_set_enabled(display, false);
		}

		~PauseDisplayIfRequired()
		{
			if (resumeRequired)
			{
				obs_display_t *display = *mDisplay;
				obs_display_set_enabled(display, true);
			}
		}
	};

};

// Looks like this really isn't needed either since there's only one "type" of display and their hWnds in windows.
class TSL_EXPORT DisplayFactory {
	const char *mTypeName;

public:
	DisplayFactory(const char *typeName) : mTypeName(typeName) {}

	DisplayContextPtr Create(HWND hWnd, uint32_t backgroundColor = 0);
};

