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

#ifndef PACKAGE_H
#define PACKAGE_H

#include <stdio.h>

int package_read_header(FILE* file, unsigned* files_count, unsigned* content_offset, unsigned* content_size);
void package_read_filename_length(FILE* file, unsigned* filename_length);
void package_read_filedata(FILE* file, unsigned filename_length, char* filename, unsigned* offset, unsigned* size);
void package_read_content(FILE* file, void* content, unsigned content_size);

void package_write_header(FILE* file, unsigned files_count);
void package_write_filedata(FILE* file, const char* filename, unsigned* filedata_offset);
void package_write_content_offset(FILE* file);
void package_write_file_content(FILE* file,
								const void* content,
								unsigned content_size,
								unsigned filedata_offset,
								unsigned* content_size_sum);
void package_write_content_size(FILE* file, unsigned content_size_sum);

#endif // PACKAGE_H
