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
#include "renderer.h"
#include "io/log.h"

int renderer_create(struct renderer* renderer)
{
	log_write(LOG_LEVEL_TRACE, "Creating renderer...");
	return 0;
}

void renderer_destroy(struct renderer* renderer)
{
	log_write(LOG_LEVEL_TRACE, "Destroying renderer...");
}

void renderer_draw(struct renderer* renderer)
{
	log_write(LOG_LEVEL_TRACE, "Updating renderer...");
}
