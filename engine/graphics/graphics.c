//
// RapidEngine - Fast and cache-friendly C game engine
// Copyright (C) 2021 Yuriy Zinchenko
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <GL/glew.h>
#include <SDL.h>
#include "graphics.h"
#include "io/log.h"

int graphics_create(struct graphics* graphics,
					const char* title,
					int width,
					int height,
					int vsync,
					int gl_major,
					int gl_minor)
{
	log_write(LOG_LEVEL_TRACE, "Creating graphics...");

	int ret = SDL_InitSubSystem(SDL_INIT_VIDEO);
	if (ret < 0)
	{
		log_write(LOG_LEVEL_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	graphics->window = SDL_CreateWindow(title,
										SDL_WINDOWPOS_CENTERED,
										SDL_WINDOWPOS_CENTERED,
										width,
										height,
										SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!graphics->window)
	{
		log_write(LOG_LEVEL_ERROR, "Failed to create SDL window: %s", SDL_GetError());
		return 1;
	}

	graphics->context = SDL_GL_CreateContext(graphics->window);
	if (!graphics->context)
	{
		log_write(LOG_LEVEL_ERROR, "Failed to create OpenGL context: %s", SDL_GetError());
		return 1;
	}

	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		log_write(LOG_LEVEL_ERROR, "Failed to initialize GLEW: %s", glewGetErrorString(glew_status));
		return 1;
	}

	if (vsync)
	{
		ret = SDL_GL_SetSwapInterval(1);
		if (ret < 0)
			log_write(LOG_LEVEL_WARNING, "Failed to set vertical sync: %s", SDL_GetError());
	}

	SDL_StartTextInput();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return 0;
}

void graphics_destroy(struct graphics* graphics)
{
	log_write(LOG_LEVEL_TRACE, "Destroying graphics...");
	SDL_StopTextInput();
	if (graphics->context)
		SDL_GL_DeleteContext(graphics->context);
	if (graphics->window)
		SDL_DestroyWindow(graphics->window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void graphics_frame_begin(struct graphics* graphics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void graphics_frame_end(struct graphics* graphics)
{
	log_write(LOG_LEVEL_TRACE, "Updating graphics...");
	SDL_GL_SwapWindow(graphics->window);
}
