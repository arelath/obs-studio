#pragma once

#include "Framework.h"
#include "obs.h"

#include <string>

class DisplayContext {
	obs_display_t *display;

public:
	inline DisplayContext(obs_display_t *display) : display(display) {}
	inline ~DisplayContext() { obs_display_destroy(display); }
	inline operator obs_display_t *() { return display; }
};

class TSL_EXPORT DisplayFactory {
	const char *mTypeName;

public:
	DisplayFactory(const char *typeName) : mTypeName(typeName) {}

	DisplayContext Create(const gs_init_data *info,
			      uint32_t backgroundColor = 0)
	{
		return obs_display_create(info, backgroundColor);
	}
};
