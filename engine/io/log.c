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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "log.h"

#define BUFFER_SIZE 512

static const char* log_levels[] = { "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "NONE" };

static FILE* g_log_file = NULL;
static enum log_level_e g_log_level = LOG_LEVEL_ERROR;

int log_open(const char* filename, enum log_level_e log_level)
{
	if (filename)
	{
		if (g_log_file)
			return 1;
		g_log_file = fopen(filename, "w");
		if (!g_log_file)
			return 1;
	}
	g_log_level = log_level;
	return 0;
}

void log_close()
{
	if (g_log_file)
	{
		fclose(g_log_file);
		g_log_file = NULL;
	}
}

void log_write(enum log_level_e log_level, const char* format, ...)
{
	if (log_level < g_log_level)
		return;

	char buffer[BUFFER_SIZE];
	int size = sprintf(buffer, "%s: ", log_levels[log_level]);
	va_list args;
	va_start(args, format);
	size += vsprintf(buffer + size, format, args);
	va_end(args);
	strcpy(buffer + size, ".\n");
	size += 2;
	fwrite(buffer, 1, size, stdout);
	if (g_log_file)
		fwrite(buffer, 1, size, g_log_file);
}
