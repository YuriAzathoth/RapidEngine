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

#include "acutest.h"

extern void test_hash_map();
extern void test_obj_queue();
extern void test_obj_vector();
extern void test_package();
extern void test_tool_package();

TEST_LIST =
{
	{ "Hash Map", test_hash_map },
	{ "Object Queue", test_obj_queue },
	{ "Object Vector", test_obj_vector },
	{ "Package", test_package },
	{ "Package Tool", test_tool_package },
	{ NULL, NULL }
};
