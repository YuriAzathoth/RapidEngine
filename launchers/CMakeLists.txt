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

SET (TARGET_NAMES client)

FOREACH (TARGET_NAME ${TARGET_NAMES})
	ADD_EXECUTABLE (${TARGET_NAME} ${TARGET_NAME}.c)
	TARGET_LINK_LIBRARIES (${TARGET_NAME} engine)
ENDFOREACH ()
