#pragma once

#include "Framework.h"
#include "ObsObjectContext.h"

MAKE_SHARED_CLASS(SceneItemContext);

class TSL_EXPORT SceneItemContext : public ObjectContext {
	obs_scene_item *source;

public:
	inline SceneItemContext(obs_scene_item *source, const std::string &name)
		: ObjectContext(name), source(source)
	{
	}

	inline ~SceneItemContext() { obs_sceneitem_release(source); }
	inline operator obs_scene_item *() { return source; }

	bool SetItemScale(const vec2 &scale)
	{
		obs_sceneitem_set_scale(source, &scale);
		return true;
	}

	bool SetItemPosition(const vec2 &position)
	{
		obs_sceneitem_set_pos(source, &position);
		return true;
	}
};
