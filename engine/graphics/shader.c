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

#include <stdlib.h>
#include <GL/glew.h>
#include "io/log.h"
#include "shader.h"

unsigned shader_compile(const char* text, enum shader_type shader_type)
{
	const GLenum SHADER_TYPES[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	const unsigned shader = glCreateShader(SHADER_TYPES[shader_type]);
	if (!shader)
		return 0;

	glShaderSource(shader, 1, (const char* const*)&text, NULL);
	glCompileShader(shader);

	int value;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &value);
	if (value != GL_TRUE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &value);
		char* log = (char*)malloc((value + 1) * sizeof(char));
		glGetShaderInfoLog(shader, value, NULL, log);
		log_write(LOG_LEVEL_ERROR, "Shader compilation error: %s", log);
		free(log);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void shader_release(unsigned shader) { glDeleteShader(shader); }

unsigned shader_create(unsigned vertex_shader, unsigned fragment_shader)
{
	const unsigned program = glCreateProgram();
	if (!program)
	{
		log_write(LOG_LEVEL_ERROR, "Failed to create shader program: %s", glGetString(glGetError()));
		return 1;
	}

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	int value;
	glGetProgramiv(program, GL_LINK_STATUS, &value);
	if (value != GL_TRUE)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &value);
		char* log = (char*)malloc((value + 1) * sizeof(char));
		log_write(LOG_LEVEL_ERROR, "Shader program linking error: %s", log);
		free(log);
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

int shader_destroy(unsigned program) { glDeleteProgram(program); }

void shader_use(unsigned program) { glUseProgram(program); }
