#define DICT_IMPLEMENTATION
#define DICT_EXTERN static inline
#define DICT_API DICT_EXTERN
/*#define DICT_USE_ALTERNATE_DICTIONARY*/
#ifdef DICT_USE_ALTERNATE_DICTIONARY
#include "alt2.h"
#endif
#include "dict.h"
#include <stdio.h>

static int get_file(void *in) { assert(in); return fgetc((FILE*)in); }
static int put_file(void *out, int ch) { assert(out); return fputc(ch, (FILE*)out); }

int main(int argc, char **argv) {
	dict_t codec = {
		.get = get_file, .put = put_file, .in = stdin, .out = stdout,
	}, *c = &codec;
	if (argc != 2)
		goto usage;
	if (!strcmp(argv[1], "c"))
		return dict_compress(c) < 0;
	if (!strcmp(argv[1], "d"))
		return dict_decompress(c) < 0;
usage:
	(void)fprintf(stderr, "usage %s c|d < infile > outfile\n", argv[0]);
	return 1;
}

