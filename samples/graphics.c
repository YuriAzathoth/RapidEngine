/*
	RapidEngine - Fast and cache-friendly C game engine
	Copyright (C) 2021-2022 Yuriy Zinchenko

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
#include <cglm/affine.h>
#include <cglm/mat4.h>
#include <cglm/quat.h>
#include <SDL2/SDL.h>
#include "graphics/camera.h"
#include "graphics/graphics.h"
#include "graphics/shader.h"

static const char* VERTEX_SHADER =
"#version 330 core\n"
"\n"
"uniform mat4 cModel;\n"
"uniform mat4 cViewProj;\n"
"\n"
"layout(location = 0) in vec3 aPos;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = cViewProj * cModel * vec4(aPos, 1.0);\n"
"}\n";

static const char* FRAGMENT_SHADER =
"#version 330 core\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.0, 0.5, 1.0, 1.0);\n"
"}\n";

int main(int argc, char** argv)
{
	int ret;
	struct graphics graphics;
	ret = graphics_create(&graphics, "Graphics Sample", 800, 600, 0, 3, 1);
	if (ret || graphics_error_check())
		return 1;

	const unsigned vertex_shader = shader_compile(VERTEX_SHADER, SHADER_TYPE_VERTEX);
	if (!vertex_shader || graphics_error_check())
		return 1;

	const unsigned fragment_shader = shader_compile(FRAGMENT_SHADER, SHADER_TYPE_FRAGMENT);
	if (!fragment_shader || graphics_error_check())
		return 1;

	const unsigned shader = shader_create(vertex_shader, fragment_shader);
	shader_release(vertex_shader);
	shader_release(fragment_shader);
	if (!shader || graphics_error_check())
		return 1;

	struct uniforms uniforms = UNIFORMS_INIT;
	shader_init(&uniforms, shader);

	struct camera camera = CAMERA_INIT;
	camera_perspective(&camera, glm_rad(45.0f), 8.0f / 6.0f, 0.1f, 100.0f);

	//    4-----------5          4-----5
	//   /|          /|          |     |
	//  / |         / |          |     |
	// 0-----------1  |    4-----0-----1-----5-----4
	// |  |        |  |    |     |     |     |     |
	// |  6--------|--7    |     |     |     |     |
	// | /         | /     6-----2-----3-----7-----6
	// |/          |/            |     |
	// 2-----------3             |     |
	//                           6-----7

	const float vertices[] =
	{
		-1.0f,  1.0f,  1.0f,	// 0
		 1.0f,  1.0f,  1.0f,	// 1
		-1.0f, -1.0f,  1.0f,	// 2
		 1.0f, -1.0f,  1.0f,	// 3
		-1.0f,  1.0f, -1.0f,	// 4
		 1.0f,  1.0f, -1.0f,	// 5
		-1.0f, -1.0f, -1.0f,	// 6
		 1.0f, -1.0f, -1.0f		// 7
	};
	const unsigned indices[] =
	{
		0, 1, 2, 1, 3, 2, // Front
		5, 4, 7, 4, 6, 7, // Back
		4, 0, 6, 0, 2, 6, // Left
		1, 5, 3, 5, 7, 3, // Right
		4, 5, 0, 5, 1, 0, // Top
		2, 3, 6, 3, 7, 6  // Bottom
	};
	const unsigned vertices_count = sizeof(vertices) / sizeof(float) / 3;
	const unsigned indices_count = sizeof(indices) / sizeof(unsigned);
	GEOMETRY_BUFFERS buffers;
	graphics_buffers_create(buffers, vertices, indices, vertices_count, indices_count);

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

		const float ticks = (float)(SDL_GetTicks());

		vec3 model_position = { 0.0f, 0.0f, 0.0f };

		mat4 model = GLM_MAT4_IDENTITY_INIT;
		glm_rotate(model, ticks * 0.001f, GLM_YUP);
		glm_translate(model, model_position);

		vec3 camera_position = { 0.0f, 0.0f, 5.0f };
		glm_vec3_copy(camera_position, camera.position);

		camera_update(&camera);

		shader_use(shader);
		shader_set_model(&uniforms, model[0]);
		shader_set_viewproj(&uniforms, camera.viewproj[0]);

		graphics_draw(buffers, indices_count);

		graphics_frame_end(&graphics);
		if (graphics_error_check())
			return 1;
	}

	shader_destroy(shader);
	graphics_buffers_destroy(buffers);
	graphics_destroy(&graphics);

	return 0;
}
