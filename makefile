CFLAGS=-Wall -Wextra -std=c99 -O2
TARGET=dict
TEST_FILE=readme.md

.PHONY: all default test clean

all default: ${TARGET}

test: ${TARGET}
	./test file ${TEST_FILE}
	cmp ${TEST_FILE} ${TEST_FILE}.orig

${TARGET}: main.c dict.h makefile
	${CC} ${CFLAGS} main.c -o ${TARGET}

rand.bin: ${TARGET}
	dd if=/dev/urandom of=rand.bin count=100
	./test file rand.bin
	cmp rand.bin rand.bin.orig

zero.bin: ${TARGET}
	dd if=/dev/zero of=zero.bin count=100
	./test file zero.bin
	cmp zero.bin zero.bin.orig

clean:
	git clean -dffx .


strings: ${TARGET}
	@./test string "This is a small string"
	@./test string "foobar"
	@./test string "the end"
	@./test string "not-a-g00d-Exampl333"
	@./test string "Smaz is a simple compression library"
	@./test string "Nothing is more difficult, and therefore more precious, than to be able to decide"
	@./test string "this is an example of what works very well with smaz"
	@./test string "1000 numbers 2000 will 10 20 30 compress very little"
	@
	@./test string "Nel mezzo del cammin di nostra vita, mi ritrovai in una selva oscura"
	@./test string "Mi illumino di immenso"
	@./test string "L'autore di questa libreria vive in Sicilia"
	@
	@./test string "http://google.com"
	@./test string "http://programming.reddit.com"
	@./test string "http://github.com/antirez/smaz/tree/master"
