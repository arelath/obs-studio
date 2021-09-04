#pragma once

#include "Framework.h"
#include "obs.h"

#include <string>

// Seems like we should be able to template this, but might get complicated. Maybe labdas for creation?
// Anyway it's probably not worth it for as few as we have

MAKE_SHARED_CLASS(SceneContext);

class TSL_EXPORT SceneContext : public ObjectContext {
	obs_scene_t *scene;

public:
	inline SceneContext(obs_scene_t *scene, const std::string & name)
		: ObjectContext(name),
		  scene(scene) {}

	inline ~SceneContext() { obs_scene_release(scene); }
	inline operator obs_scene_t *() { return scene; }
};

class TSL_EXPORT SceneFactory {
public:
	virtual SceneContextPtr Create(const std::string &name)
	{
		return std::make_shared<SceneContext>(obs_scene_create(name.c_str()), name);
	}
};
