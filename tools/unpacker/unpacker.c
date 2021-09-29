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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io/package.h"

#define BUFFER_SIZE 1024

static void show_help()
{
	printf("Usage: unpacker [options]\n");
	printf("Options:\n");
	printf("  --help          Display this information.\n");
	printf("  -i file         Specify input package.\n");
	printf("  -o file         Specify unpacked files output directory.\n");
}

int main(int argc, char** argv)
{
	char* input_file = NULL;
	char* output_directory = NULL;
	char* argchr;
	for (char** argptr = argv + 1; argptr < argv + argc; ++argptr)
	{
		argchr = *argptr;
		if (*argchr == '-')
		{
			++argchr;
			if (*argchr == '-')
			{
				++argchr;
				if (!strcmp(argchr, "help"))
				{
					show_help();
					return 0;
				}
			}
			else if (*argchr == 'i')
				input_file = *++argptr;
			else if (*argchr == 'o')
				output_directory = *++argptr;
		}
	}

	if (!input_file)
	{
		fprintf(stderr, "unpacker: fatal error: no input file\n");
		return 1;
	}
	if (!output_directory)
	{
		fprintf(stderr, "unpacker: fatal error: no output directory\n");
		return 1;
	}

	FILE* package = fopen(input_file, "rb");
	if (!package)
	{
		fprintf(stderr, "unpacker: fatal error: failed to open input package file\n");
		return 1;
	}

	unsigned files_count, content_offset, content_size;
	if (package_read_header(package, &files_count, &content_offset, &content_size) || !files_count)
	{
		fprintf(stderr, "unpacker: fatal error: invalid or corrupted input package file\n");
		return 1;
	}

	unsigned filename_length;
	char** filenames = (char**)malloc(files_count * sizeof(char*));
	unsigned* file_offsets = (unsigned*)malloc(files_count * sizeof(unsigned));
	unsigned* file_sizes = (unsigned*)malloc(files_count * sizeof(unsigned));
	for (unsigned i = 0; i < files_count; ++i)
	{
		package_read_filename_length(package, &filename_length);
		filenames[i] = (char*)malloc(filename_length * sizeof(char));
		package_read_filedata(package, filename_length, filenames[i], &file_offsets[i], &file_sizes[i]);
	}

	void* content = malloc(content_size);
	package_read_content(package, content, content_size);

	FILE* file;
	char filename_buffer[BUFFER_SIZE];
	for (unsigned i = 0; i < files_count; ++i)
	{
		sprintf(filename_buffer, "%s/%s", output_directory, filenames[i]);
		file = fopen(filename_buffer, "wb");
		fwrite(content + file_offsets[i], file_sizes[i], 1, file);
		fclose(file);
	}

	free(content);
	free(file_offsets);
	free(file_sizes);
	for (unsigned i = 0; i < files_count; ++i)
		free(filenames[i]);
	free(filenames);
	fclose(package);

	return 0;
}
