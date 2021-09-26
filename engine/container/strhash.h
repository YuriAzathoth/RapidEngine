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

#ifndef STRHASH_H
#define STRHASH_H

#define HASH_PRIME 65

inline static unsigned long long strhash(const char* str)
{
	unsigned long long hash = 0;
	unsigned long long pow = 1;
	for (const char* ptr = str; *ptr != '\0'; ++ptr)
	{
		hash += (*ptr - ' ' + 1) * pow;
		pow *= HASH_PRIME;
	}
	return hash;
}

#endif // STRHASH_H
