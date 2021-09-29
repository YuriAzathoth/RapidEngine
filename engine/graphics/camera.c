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

#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/quat.h>
#include <GL/glew.h>
#include "camera.h"
#include "io/log.h"

void camera_perspective(struct camera* camera, float fov, float aspect, float nearz, float farz)
{
	glm_perspective(fov, aspect, nearz, farz, camera->proj);
}

void camera_update(struct camera* camera)
{
	vec3 forward, up;
	glm_quat_rotatev(camera->rotation, GLM_FORWARD, forward);
	glm_quat_rotatev(camera->rotation, GLM_YUP, up);

	mat4 view;
	glm_look(camera->position, forward, up, view);

	glm_mat4_mul_sse2(camera->proj, view, camera->viewproj);
}
