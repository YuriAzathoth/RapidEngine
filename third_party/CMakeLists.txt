#
# RapidEngine - Fast and cache-friendly C game engine
# Copyright (C) 2021 Yuriy Zinchenko
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

SET (TARGET_NAME third_party)

FILE (GLOB_RECURSE SOURCE_FILES *.c)
FILE (GLOB_RECURSE HEADER_FILES *.h)

ADD_LIBRARY (${TARGET_NAME} INTERFACE ${SOURCE_FILES} ${HEADER_FILES})

TARGET_INCLUDE_DIRECTORIES (${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
