/*
	RapidEngine - Fast and cache-friendly C game engine
	Copyright (C) 2021 Yuriy Zinchenko

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <SDL.h>
#include "engine.h"
#include "io/log.h"

int engine_create(struct engine* engine)
{
	int ret;
	{
		const char* filename = "engine.log";
		const enum log_level_e log_level = LOG_LEVEL_TRACE;
		ret = log_open(filename, log_level);
		if (ret != 0)
			return ret;
	}
	log_write(LOG_LEVEL_TRACE, "Creating engine...");
	{
		ret = filesystem_create(&engine->filesystem);
		if (ret != 0)
			return ret;
	}
	{
		const char* title = "Rapid Engine";
		const int width = 1024;
		const int height = 768;
		const int vsync = 0;
		const int gl_major = 3;
		const int gl_minor = 1;
		ret = graphics_create(&engine->graphics, title, width, height, vsync, gl_major, gl_minor);
		if (ret != 0)
			return ret;
	}
	{
		ret = renderer_create(&engine->renderer);
		if (ret != 0)
			return ret;
	}
	engine->run = 1;
	return 0;
}

void engine_destroy(struct engine* engine)
{
	log_write(LOG_LEVEL_TRACE, "Destroying engine...");
	renderer_destroy(&engine->renderer);
	graphics_destroy(&engine->graphics);
	filesystem_destroy(&engine->filesystem);
	log_close();
}

void engine_run(struct engine* engine)
{
	log_write(LOG_LEVEL_TRACE, "Running engine...");
	SDL_Event event;
	while (engine->run)
	{
		while (SDL_PollEvent(&event))
			switch (event.type)
			{
			case SDL_QUIT:
				engine->run = 0;
				break;
			}

		graphics_frame_begin(&engine->graphics);
		renderer_draw(&engine->renderer);
		graphics_frame_end(&engine->graphics);
	}
	log_write(LOG_LEVEL_TRACE, "Running engine...");
}
