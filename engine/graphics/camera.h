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

#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/quat.h>
#include <cglm/types.h>
#include "core/common.h"

#define CAMERA_INIT { GLM_MAT4_IDENTITY_INIT, GLM_MAT4_IDENTITY_INIT, GLM_QUAT_IDENTITY_INIT, GLM_VEC3_ZERO_INIT }

#define camera_perspective(camera, fov, aspect, nearz, farz) glm_perspective(fov, aspect, nearz, farz, (camera)->proj)

struct camera
{
	mat4 viewproj;
	mat4 proj;
	versor rotation;
	vec3 position;
};

ENGINE_INLINE void camera_update(struct camera* camera)
{
	vec3 forward, up;
	glm_quat_rotatev(camera->rotation, GLM_FORWARD, forward);
	glm_quat_rotatev(camera->rotation, GLM_YUP, up);

	mat4 view;
	glm_look(camera->position, forward, up, view);

	glm_mat4_mul_sse2(camera->proj, view, camera->viewproj);
}

#endif // CAMERA_H
