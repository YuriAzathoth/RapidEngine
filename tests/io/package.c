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

#define TEST_NO_MAIN
#include <stdio.h>
#include <stdlib.h>
#include "acutest.h"
#include "io/package.h"

#define FILES_COUNT 4
#define TEMPORARY_DIR "temp"
#define TEMPORARY_FILE TEMPORARY_DIR "/package.pak"

void test_package()
{
	const char* file_names[] =
	{
		"text_file_1.txt",
		"binary_file_1.bin",
		"text_file_2.txt",
		"binary_file_2.bin"
	};
	const char* text_file_contents[] =
	{
		"Hello, World!",
		"C++ is muche better, than Java and C#!"
	};
	const unsigned binary_file_content_1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const char binary_file_content_2[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const void* file_contents[] =
	{
		binary_file_content_1,
		text_file_contents[0],
		binary_file_content_2,
		text_file_contents[1]
	};
	const unsigned file_sizes[] =
	{
		(unsigned)sizeof(binary_file_content_1),
		(unsigned)strlen(text_file_contents[0]),
		(unsigned)sizeof(binary_file_content_2),
		(unsigned)strlen(text_file_contents[1])
	};

	unsigned write_offsets[FILES_COUNT];
	unsigned write_content_size = 0;

	mkdir(TEMPORARY_DIR);
	FILE* file = fopen(TEMPORARY_FILE, "wb");
	package_write_header(file, FILES_COUNT);
	for (unsigned i = 0; i < FILES_COUNT; ++i)
		package_write_filedata(file, file_names[i], &write_offsets[i]);
	package_write_content_offset(file);
	for (unsigned i = 0; i < FILES_COUNT; ++i)
		package_write_file_content(file, file_contents[i], file_sizes[i], write_offsets[i], &write_content_size);
	package_write_content_size(file, write_content_size);
	fclose(file);

	unsigned read_offsets[FILES_COUNT];
	unsigned read_sizes[FILES_COUNT];
	unsigned read_files_count, read_content_block_offset, read_content_size, filename_length;
	char *filename, *read_content;

	file = fopen(TEMPORARY_FILE, "rb");
	TEST_ASSERT(!package_read_header(file, &read_files_count, &read_content_block_offset, &read_content_size));
	TEST_ASSERT(read_files_count == FILES_COUNT);
	TEST_ASSERT(read_content_size == write_content_size);
	for (unsigned i = 0; i < read_files_count; ++i)
	{
		package_read_filename_length(file, &filename_length);
		TEST_ASSERT(filename_length == strlen(file_names[i]));
		filename = (char*)malloc(filename_length + 1);
		package_read_filedata(file, filename_length, filename, &read_offsets[i], &read_sizes[i]);
		TEST_ASSERT(!strcmp(filename, file_names[i]));
		free(filename);
	}
	read_content = (char*)malloc(read_content_size);
	package_read_content(file, read_content, read_content_size);
	for (unsigned i = 0; i < read_files_count; ++i)
		TEST_ASSERT(!memcmp(&read_content[read_offsets[i] - read_content_block_offset], file_contents[i], read_sizes[i]));
	free(read_content);
	fclose(file);

	remove(TEMPORARY_FILE);
	rmdir(TEMPORARY_DIR);
}
