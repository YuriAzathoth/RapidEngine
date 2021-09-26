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

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "graphics/graphics.h"
#include "graphics/shader.h"

#include <GL/glew.h>

static const char* VERTEX_SHADER =
"#version 330 core\n"
"\n"
"layout(location = 0) in vec3 aPos;\n"
"\n"
"out vec4 vertexColor;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    vertexColor = vec4(aPos, 1.0);\n"
"}\n";

static const char* FRAGMENT_SHADER =
"#version 330 core\n"
"\n"
"in vec4 vertexColor;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vertexColor;\n"
"}\n";

int main(int argc, char** argv)
{
	int ret;
	struct graphics graphics;
	ret = graphics_create(&graphics, "Graphics Sample", 800, 600, 0, 3, 1);
	if (ret || graphics_error_check())
		return 1;

	const unsigned vertex_shader = shader_compile(VERTEX_SHADER, sizeof(VERTEX_SHADER), SHADER_TYPE_VERTEX);
	if (!vertex_shader || graphics_error_check())
		return 1;

	const unsigned fragment_shader = shader_compile(FRAGMENT_SHADER, sizeof(FRAGMENT_SHADER), SHADER_TYPE_FRAGMENT);
	if (!fragment_shader || graphics_error_check())
		return 1;

	const unsigned shader = shader_create(vertex_shader, fragment_shader);
	shader_release(vertex_shader);
	shader_release(fragment_shader);
	if (!shader || graphics_error_check())
		return 1;

	shader_use(shader);
	if (graphics_error_check())
		return 1;

	const float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.0f
	};
	const unsigned indices[] = { 0, 1, 2, 1, 2, 3 };
	GEOMETRY_BUFFERS buffers;
	graphics_buffers_create(buffers);
	graphics_buffers_load(buffers, vertices, indices, 6);

	SDL_Event event;
	int run = 1;
	while (run)
	{
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				run = 0;

		graphics_frame_begin(&graphics);
		if (graphics_error_check())
			return 1;

		graphics_draw(buffers, 6);

		graphics_frame_end(&graphics);
		if (graphics_error_check())
			return 1;
	}

	graphics_buffers_destroy(buffers);

	graphics_destroy(&graphics);

	return 0;
}
