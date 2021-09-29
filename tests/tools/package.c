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

#define TEST_NO_MAIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "acutest.h"
#include "io/package.h"

#define FILES_COUNT 4
#define TEMPORARY_DIR "temp"
#define TEMPORARY_FILE "file.txt"
#define TEMPORARY_PACKAGE "package.pak"
#define PACK_COMMAND " -o " TEMPORARY_PACKAGE " " TEMPORARY_FILE
#define UNPACK_COMMAND " -i " TEMPORARY_DIR "/" TEMPORARY_PACKAGE " -o " TEMPORARY_DIR "/" TEMPORARY_DIR

void test_tool_package()
{
	const char in_file_content[] = "Hello, World!";
	const size_t in_file_size = sizeof(in_file_content) - 1;

	mkdir(TEMPORARY_DIR);

	FILE* file = fopen(TEMPORARY_DIR "/" TEMPORARY_FILE, "wb");
	fwrite(in_file_content, 1, in_file_size, file);
	fclose(file);

	chdir(TEMPORARY_DIR);
#ifdef WIN32
	TEST_ASSERT(!system("..\\packer.exe" PACK_COMMAND));
#else
	TEST_ASSERT(!system("../packer -o " PACK_COMMAND));
#endif // WIN32
	chdir("..");

	remove(TEMPORARY_FILE);

#ifdef WIN32
	TEST_ASSERT(!system("unpacker.exe" UNPACK_COMMAND));
#else
	TEST_ASSERT(!system("./unpacker" UNPACK_COMMAND));
#endif // WIN32

	file = fopen(TEMPORARY_DIR "/" TEMPORARY_FILE, "rb");
	fseek(file, 0, SEEK_END);
	const unsigned out_file_size = (unsigned)ftell(file);
	rewind(file);
	void* out_file_content = malloc(out_file_size);
	fread(out_file_content, 1, out_file_size, file);
	TEST_ASSERT(out_file_size == in_file_size);
	TEST_ASSERT(!memcmp(in_file_content, out_file_content, in_file_size));
	fclose(file);

	chdir(TEMPORARY_DIR);
	remove(TEMPORARY_FILE);
	remove(TEMPORARY_PACKAGE);
	chdir("..");
	rmdir(TEMPORARY_DIR);
}
