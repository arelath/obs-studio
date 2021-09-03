#pragma once

#include "Framework.h"
#include "obs.h"

#include <string>

// Seems like we should be able to template this, but might get complicated. Maybe labdas for creation?
// Anyway it's probably not worth it for as few as we have

class TSL_EXPORT SceneContext {
	obs_scene_t *scene;

public:
	inline SceneContext(obs_scene_t *scene) : scene(scene) {}
	inline ~SceneContext() { obs_scene_release(scene); }
	inline operator obs_scene_t *() { return scene; }
};

/* --------------------------------------------------- */

class DisplayContext {
	obs_display_t *display;

public:
	inline DisplayContext(obs_display_t *display) : display(display) {}
	inline ~DisplayContext() { obs_display_destroy(display); }
	inline operator obs_display_t *() { return display; }
};
