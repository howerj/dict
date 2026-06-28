CFLAGS=-Wall -Wextra -std=c99 -O2
TARGET=dict
TEST_FILE=readme.md

all default: ${TARGET}

test: ${TARGET}
	./${TARGET} c < ${TEST_FILE} > ${TEST_FILE}.x
	./${TARGET} d < ${TEST_FILE}.x > ${TEST_FILE}.orig
	cmp ${TEST_FILE} ${TEST_FILE}.orig

${TARGET}: main.c dict.h makefile
	${CC} ${CFLAGS} main.c -o ${TARGET}

clean:
	git clean -dffx .
