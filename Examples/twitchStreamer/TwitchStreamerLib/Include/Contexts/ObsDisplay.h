#pragma once

#include "Framework.h"
#include "ObsObjectContext.h"

#include <string>

MAKE_SHARED_CLASS(DisplayContext);

class DisplayContext : public ObjectContext {
	obs_display_t *display;

public:
	inline DisplayContext(HWND hWnd, uint32_t backgroundColor = 0);
	inline ~DisplayContext() { obs_display_destroy(display); }
	inline operator obs_display_t *() { return display; }


};

// Looks like this really isn't needed either since there's only one "type" of display and their hWnds in windows.
class TSL_EXPORT DisplayFactory {
	const char *mTypeName;

public:
	DisplayFactory(const char *typeName) : mTypeName(typeName) {}

	DisplayContextPtr Create(HWND hWnd, uint32_t backgroundColor = 0);
};

