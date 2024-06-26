#include <SDL2/SDL.h>
#include <sdl2_app.h>
#include <types/bundle.h>
#include <types/primitive.h>

SDL_SceneDesc export_sdl_scene(void);

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	const char *title = argc >= 1 ? argv[0] : "NO_TITLE";
	SDL_App app;
	app.window = SDL_CreateWindow(title, -1, -1, -1, -1, SDL_WINDOW_RESIZABLE);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_RendererInfo renderer_info;
	SDL_GetRendererInfo(app.renderer, &renderer_info);
	u64 old_ticks = SDL_GetPerformanceCounter();
	const u64 ticks_per_second = SDL_GetPerformanceFrequency();

	/* Maximime window in full screen area. */
	int top, left, bottom, right;
	SDL_GetWindowBordersSize(app.window, &top, &left, &bottom, &right);
	SDL_Rect bounds;
	SDL_GetDisplayUsableBounds(0, &bounds);
	const SDL_Rect display = { bounds.x, bounds.y + top, bounds.w, bounds.h - top };
	SDL_SetWindowPosition(app.window, display.x, display.y);
	SDL_SetWindowSize(app.window, display.w, display.h);

	SDL_SceneDesc scene = export_sdl_scene();
	SDL_SceneData *scene_data = SDL_malloc(scene.data_size);
	scene.init(&app, scene_data);

	bool running = true;
	while (running) {
		const u64 new_ticks = SDL_GetPerformanceCounter();
		const u64 diff_ticks = new_ticks - old_ticks;
		const f64seconds delta_time = { (f64)diff_ticks / (f64)ticks_per_second };
		old_ticks = new_ticks;

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
			};
			scene.handle_sdl_event(&app, scene_data, &event);
		}

		int w, h;
		SDL_GetWindowSize(app.window, &w, &h);
		scene.tick(&app, scene_data, delta_time, (f32v2){ w, h });
		SDL_RenderPresent(app.renderer);
		SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
		SDL_RenderClear(app.renderer);

		char title_fps[1024];
		int fps = (int)(1.0 / delta_time.seconds);
		snprintf(title_fps, sizeof(title_fps), "%s (%s: %ifps)", title, renderer_info.name, fps);
		SDL_SetWindowTitle(app.window, title_fps);
	}

	scene.fini(&app, scene_data);
	SDL_free(scene_data);

	SDL_Quit();
	return 0;
}
