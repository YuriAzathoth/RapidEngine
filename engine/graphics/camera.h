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

#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/types.h>

#define CAMERA_INIT { GLM_MAT4_IDENTITY_INIT, GLM_MAT4_IDENTITY_INIT, GLM_QUAT_IDENTITY_INIT, GLM_VEC3_ZERO_INIT }

struct camera
{
	mat4 viewproj;
	mat4 proj;
	versor rotation;
	vec3 position;
};

void camera_perspective(struct camera* camera, float fov, float aspect, float nearz, float farz);
void camera_update(struct camera* camera);

#endif // CAMERA_H
