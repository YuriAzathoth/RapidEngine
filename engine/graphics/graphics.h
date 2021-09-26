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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define GRAPHICS_INIT { 0, 0 }

typedef struct SDL_Window SDL_Window;
typedef void *SDL_GLContext;

struct graphics
{
	SDL_Window* window;
	SDL_GLContext context;
};

int graphics_create(struct graphics* graphics,
					const char* title,
					int width,
					int height,
					int vsync,
					int gl_major,
					int gl_minor);

void graphics_destroy(struct graphics* graphics);

void graphics_frame_begin(struct graphics* graphics);
void graphics_frame_end(struct graphics* graphics);

#endif // GRAPHICS_H
