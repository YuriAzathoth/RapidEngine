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

#include <stdlib.h>
#include <string.h>
#include "package.h"
#include "io/log.h"

#define PACKAGE_MAGIC_NUMBER 0x20d04fe0

int package_read_header(FILE* file, unsigned* files_count, unsigned* content_offset, unsigned* content_size)
{
	unsigned magic_number;
	fread(&magic_number, sizeof(unsigned), 1, file);
	const int valid = magic_number == PACKAGE_MAGIC_NUMBER;
	if (valid)
	{
		fread(files_count, sizeof(unsigned), 1, file);
		fread(content_offset, sizeof(unsigned), 1, file);
		fread(content_size, sizeof(unsigned), 1, file);
	}
	return !valid;
}

void package_read_filename_length(FILE* file, unsigned* filename_length)
{
	fread(filename_length, sizeof(unsigned), 1, file);
}

void package_read_filedata(FILE* file, unsigned filename_length, char* filename, unsigned* offset, unsigned* size)
{
	fread(filename, 1, filename_length, file);
	filename[filename_length] = '\0';
	fread(offset, sizeof(unsigned), 1, file);
	fread(size, sizeof(unsigned), 1, file);
}

void package_read_content(FILE* file, void* content, unsigned content_size)
{
	fread(content, 1, content_size, file);
}

void package_write_header(FILE* file, unsigned files_count)
{
	const unsigned magic_number = PACKAGE_MAGIC_NUMBER;
	const unsigned zero_bytes = 0;
	fwrite(&magic_number, sizeof(unsigned), 1, file);
	fwrite(&files_count, sizeof(unsigned), 1, file);
	fwrite(&zero_bytes, sizeof(unsigned), 1, file);
	fwrite(&zero_bytes, sizeof(unsigned), 1, file);
}

void package_write_filedata(FILE* file, const char* filename, unsigned* filedata_offset)
{
	const unsigned filename_length = strlen(filename);
	const unsigned zero_bytes = 0;
	fwrite(&filename_length, sizeof(unsigned), 1, file);
	fwrite(filename, 1, filename_length, file);
	*filedata_offset = (unsigned)ftell(file);
	fwrite(&zero_bytes, sizeof(unsigned), 1, file);
	fwrite(&zero_bytes, sizeof(unsigned), 1, file);
}

void package_write_content_offset(FILE* file)
{
	const unsigned content_block_offset = (unsigned)ftell(file);
	fseek(file, 2 * sizeof(unsigned), SEEK_SET);
	fwrite(&content_block_offset, sizeof(unsigned), 1, file);
	fseek(file, 0, SEEK_END);
}

void package_write_file_content(FILE* file,
								const void* content,
								unsigned content_size,
								unsigned filedata_offset,
								unsigned* content_size_sum)
{
	const unsigned content_offset = (unsigned)ftell(file);
	fwrite(content, 1, content_size, file);
	fseek(file, (long)filedata_offset, SEEK_SET);
	fwrite(&content_offset, sizeof(unsigned), 1, file);
	fwrite(&content_size, sizeof(unsigned), 1, file);
	fseek(file, 0, SEEK_END);
	*content_size_sum += content_size;
}

void package_write_content_size(FILE* file, unsigned content_size_sum)
{
	fseek(file, 3 * sizeof(unsigned), SEEK_SET);
	fwrite(&content_size_sum, sizeof(unsigned), 1, file);
}
