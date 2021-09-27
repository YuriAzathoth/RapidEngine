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

#include <GL/glew.h>
#include <SDL.h>
#include "graphics.h"
#include "io/log.h"

enum buffer_index_e
{
	ATTRIB_ARRAY,
	VERTEX_ARRAY,
	ELEMENT_ARRAY
};

static const char* ERROR_STRINGS[] =
{
	"Invalid enum",
	"Invalid value",
	"Invalid operation",
	"Stack overflow",
	"Stack underflow",
	"Out of memory",
	"Invalid framebuffer operation",
	"No error",
	"Unknown error"
};

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
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		return 1;
	}

	graphics->context = SDL_GL_CreateContext(graphics->window);
	if (!graphics->context)
	{
		log_write(LOG_LEVEL_ERROR, "Failed to create OpenGL context: %s", SDL_GetError());
		SDL_DestroyWindow(graphics->window);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		return 1;
	}

	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		log_write(LOG_LEVEL_ERROR, "Failed to initialize GLEW: %s", glewGetErrorString(glew_status));
		SDL_GL_DeleteContext(graphics->context);
		SDL_DestroyWindow(graphics->window);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		return 1;
	}

	if (vsync)
	{
		ret = SDL_GL_SetSwapInterval(1);
		if (ret < 0)
			log_write(LOG_LEVEL_WARNING, "Failed to set vertical sync: %s", SDL_GetError());
	}

	SDL_StartTextInput();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return 0;
}

void graphics_destroy(struct graphics* graphics)
{
	log_write(LOG_LEVEL_TRACE, "Destroying graphics...");
	SDL_StopTextInput();
	SDL_GL_DeleteContext(graphics->context);
	SDL_DestroyWindow(graphics->window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void graphics_frame_begin(struct graphics* graphics)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void graphics_frame_end(struct graphics* graphics)
{
	SDL_GL_SwapWindow(graphics->window);
}

void graphics_buffers_create(unsigned* buffers,
							 const float* vertices,
							 const unsigned* indices,
							 unsigned vertices_count,
							 unsigned indices_count)
{
	glGenVertexArrays(1, &buffers[ATTRIB_ARRAY]);
	glGenBuffers(2, &buffers[VERTEX_ARRAY]);

	glBindVertexArray(buffers[ATTRIB_ARRAY]);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_ARRAY]);
	glBufferData(GL_ARRAY_BUFFER, vertices_count * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[ELEMENT_ARRAY]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(unsigned), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void graphics_buffers_destroy(unsigned* buffers)
{
	glDeleteBuffers(2, &buffers[VERTEX_ARRAY]);
	glDeleteVertexArrays(1, &buffers[ATTRIB_ARRAY]);
}

void graphics_draw(unsigned* buffers, unsigned indices_count)
{
	glBindVertexArray(buffers[ATTRIB_ARRAY]);
	glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned graphics_error_check()
{
	GLenum error;
	unsigned success = 1;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		log_write(LOG_LEVEL_ERROR, "OpenGL error: %s", graphics_error_string(error));
		success = 0;
	}
	return !success;
}

const char* graphics_error_string(unsigned error_code)
{
	switch (error_code)
	{
	case GL_INVALID_ENUM:
		return ERROR_STRINGS[0];
		break;
	case GL_INVALID_VALUE:
		return ERROR_STRINGS[1];
		break;
	case GL_INVALID_OPERATION:
		return ERROR_STRINGS[2];
		break;
	case GL_STACK_OVERFLOW:
		return ERROR_STRINGS[3];
		break;
	case GL_STACK_UNDERFLOW:
		return ERROR_STRINGS[4];
		break;
	case GL_OUT_OF_MEMORY:
		return ERROR_STRINGS[5];
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return ERROR_STRINGS[6];
		break;
	case GL_NO_ERROR:
		return ERROR_STRINGS[7];
		break;
	default:
		return ERROR_STRINGS[8];
		break;
	}
}
