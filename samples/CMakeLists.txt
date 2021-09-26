#
# Copyright (c) 2021 Yuriy Zinchenko.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

FILE (GLOB_RECURSE SAMPLE_FILES *.c)

FOREACH (SAMPLE_FILE ${SAMPLE_FILES})
	GET_FILENAME_COMPONENT (SAMPLE_NAME ${SAMPLE_FILE} NAME)
	STRING (REPLACE ".c" "" SAMPLE_NAME ${SAMPLE_NAME})
	SET (TARGET_NAME sample_${SAMPLE_NAME})
	ADD_EXECUTABLE (${TARGET_NAME} ${SAMPLE_FILE})
	TARGET_LINK_LIBRARIES (${TARGET_NAME} PRIVATE engine)
ENDFOREACH ()
