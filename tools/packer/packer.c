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

static void show_help()
{
	printf("Usage: packer [options] file...\n");
	printf("Options:\n");
	printf("  --help          Display this information.\n");
	printf("  -o file         Specify output package filename.\n");
}

int main(int argc, char** argv)
{
	char** input_files = NULL;
	unsigned input_count = 0;
	char* output_file = NULL;
	char* argchr;
	int cont;
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
			else if (*argchr == 'o')
				output_file = *++argptr;
			cont = 0;
		}
		else
		{
			if (!cont)
			{
				input_count = 1;
				input_files = argptr;
			}
			else
				++input_count;
			cont = 1;
		}
	}

	if (!input_count)
	{
		fprintf(stderr, "packer: fatal error: no input files\n");
		return 1;
	}
	if (!output_file)
	{
		fprintf(stderr, "packer: fatal error: no output filename\n");
		return 1;
	}

	FILE* package = fopen(output_file, "wb");
	if (!package)
	{
		fprintf(stderr, "packer: fatal error: failed to create output package file\n");
		return 1;
	}

	unsigned* offset_positions = (unsigned*)malloc(sizeof(unsigned));

	package_write_header(package, input_count);

	for (unsigned i = 0; i < input_count; ++i)
		package_write_filedata(package, input_files[i], &offset_positions[i]);
	package_write_content_offset(package);

	FILE* file;
	unsigned file_size;
	unsigned file_content_size = 0;
	unsigned content_size = 0;
	void* file_content = NULL;
	for (unsigned i = 0; i < input_count; ++i)
	{
		file = fopen(input_files[i], "rb");
		fseek(file, 0, SEEK_END);
		file_size = (unsigned)ftell(file);
		rewind(file);
		if (file_size > file_content_size)
		{
			free(file_content);
			file_content_size = file_size;
			file_content = malloc(file_content_size);
		}
		fread(file_content, 1, file_content_size, file);
		fclose(file);
		package_write_file_content(package, file_content, file_size, offset_positions[i], &content_size);
	}
	free(file_content);
	free(offset_positions);

	package_write_content_size(package, content_size);
	fclose(package);

	return 0;
}
