
#include <Windows.h>
#include "Contexts/ObsDisplay.h"

using namespace std;

static void RenderWindow(void *data, uint32_t cx, uint32_t cy);

DisplayContextPtr DisplayFactory::Create(HWND hWnd,
					 uint32_t backgroundColor)
{
	DisplayContextPtr display = make_shared<DisplayContext>(hWnd, backgroundColor);

	return display;
}

inline DisplayContext::DisplayContext(HWND hWnd, uint32_t backgroundColor)
	: ObjectContext("Display")
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	gs_init_data info = {};
	info.cx = rc.right;
	info.cy = rc.bottom;
	info.format = GS_RGBA;
	info.zsformat = GS_ZS_NONE;
	info.window.hwnd = hWnd;

	display = obs_display_create(&info, backgroundColor);
	obs_display_add_draw_callback(display, RenderWindow, this);
}

static void RenderWindow(void *data, uint32_t cx, uint32_t cy)
{
	obs_render_main_texture();

	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(cx);
	UNUSED_PARAMETER(cy);
}
