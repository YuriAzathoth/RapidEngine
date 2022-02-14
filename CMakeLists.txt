#
# RapidEngine - Fast and cache-friendly C game engine
# Copyright (C) 2021-2022 Yuriy Zinchenko
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

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.12)

IF (COMMAND CMAKE_POLICY)
	CMAKE_POLICY (SET CMP0048 NEW)
ENDIF ()

PROJECT (RapidEngine VERSION 0.0.0.0)

SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET (CMAKE_INSTALL_INCLUDE_DIRECTORY D:/include/${CMAKE_PROJECT_NAME})
SET (CMAKE_INSTALL_CONFIG_DIRECTORY D:/lib/cmake/${CMAKE_PROJECT_NAME})

FIND_PACKAGE (GLEW REQUIRED)
FIND_PACKAGE (SDL2 REQUIRED)

OPTION (BUILD_SAMPLES "Enable building samples" ON)
OPTION (BUILD_TESTS "Enable building tests" ON)
MARK_AS_ADVANCED (BUILD_SAMPLES)
MARK_AS_ADVANCED (BUILD_TESTS)

ADD_SUBDIRECTORY (engine)
ADD_SUBDIRECTORY (launchers)
ADD_SUBDIRECTORY (tools)
IF (BUILD_SAMPLES)
	ADD_SUBDIRECTORY (samples)
ENDIF ()
IF (BUILD_TESTS)
	ADD_SUBDIRECTORY (tests)
ENDIF (BUILD_TESTS)
