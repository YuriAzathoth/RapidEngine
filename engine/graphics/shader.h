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

#ifndef SHADER_H
#define SHADER_H

#define UNIFORMS_INIT { 0, 0 }

struct uniforms
{
	unsigned model;
	unsigned viewproj;
};

enum shader_type
{
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_FRAGMENT
};

unsigned shader_compile(const char* text, enum shader_type shader_type);
void shader_release(unsigned shader);

unsigned shader_create(unsigned vertex_shader, unsigned fragment_shader);
int shader_destroy(unsigned program);
void shader_use(unsigned program);

int shader_init(struct uniforms* uniforms, unsigned program);
void shader_set_model(struct uniforms* uniforms, const float* model);
void shader_set_viewproj(struct uniforms* uniforms, const float* viewproj);

#endif // SHADER_H
