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

#ifndef ENGINE_H
#define ENGINE_H

#include "graphics/graphics.h"
#include "graphics/renderer.h"

#define ENGINE_INIT { GRAPHICS_INIT, RENDERER_INIT, 0 }

struct engine
{
	struct graphics graphics;
	struct renderer renderer;
	int run;
};

int engine_create(struct engine* engine);
void engine_destroy(struct engine* engine);
void engine_run(struct engine* engine);

#endif // ENGINE_H
