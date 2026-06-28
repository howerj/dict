#ifndef DICT_H
#define DICT_H
#define DICT_AUTHOR "Richard James Howe"
#define DICT_LICENSE "0BSD / Public Domain"
#define DICT_EMAIL "howe.r.j.89@gmail.com"
#define DICT_PROJECT "A header-only fixed dictionary compression CODEC"
#define DICT_VERSION ("v2.0.0")
#define DICT_REPO "https://github.com/howerj/dict"

#include <assert.h>
#include <string.h>

#ifndef DICT_EXTERN /* applied to exported functions during declaration */
#define DICT_EXTERN extern
#endif

#ifndef DICT_API /* applied to exported functions during implementation */
#define DICT_API
#endif

#ifndef DICT_XMACRO
#define DICT_XMACRO\
	X(" ") X("e") X("t") X("a") X("i") X("o") X("n") X("r") \
	X("s") X("l") X("h") X("d") X("c") X("u") X("e ") X("]") \
	X("[") X("m") X("p") X("g") X(" t") X("f") X("th") X("s ") \
	X("he") X(" a") X("in") X("er") X("\n") X("an") X("y") X("n ") \
	X("b") X("d ") X(" th") X("on") X("'") X("]]") X("[[") X("w") \
	X("the") X("  ") X("he ") X("re") X(" o") X(".") X(",") X("t ") \
	X("or") X(", ") X("ti") X("at") X("te") X("en") X("es") X("v") \
	X(" [") X(" i") X("al") X(" [[") X("is") X("ar") X("nd") X("   ") \
	X("st") X("r ") X("ed") X(";") X("y ") X("nt") X("it") X(" s") \
	X("of") X("f ") X(" of") X("le") X("ri") X("to") X("of ") X(" c") \
	X("&") X("ng") X("''") X("io") X("ic") X("me") X("as") X(" w") \
	X("ed ") X("|") X("se") X("co") X("/") X("1") X("and") X("t;") \
	X("ra") X(" b") X(" an") X("o ") X("k") X("=") X("de") X("ion") \
	X("ve") X(". ") X(" in") X("ro") X("nd ") X("ot") X(" f") X("l ") \
	X("0") X("li") X(" p") X("la") X("] ") X("om") X("C") X("ing") \
	X("A") X("ha") X("]] ") X("ne") X(":") X("ce") X("ea") X("-") \
	X("si") X("ta") X("ma") X("a ") X("ou") X("hi") X("h ") X("T") \
	X(" m") X("ll") X("am") X("in ") X("el") X("S") X(" h") X("ca") \
	X("tio") X("g ") X("ch") X("ent") X(" d") X(" to") X("er ") X("us") \
	X("ng ") X("on ") X("<") X(">") X("ns") X("2") X("to ") X("q") \
	X("na") X(" r") X("tr") X("qu") X("I") X("ge") X("di") X("is ") \
	X("ur") X("9") X(" e") X("be") X("ec") X("ia") X("ni") X("*") \
	X("ol") X("ac") X("il") X("]\n") X("es ") X("ct") X("al ") X(" co") \
	X("as ") X("x") X("rs") X(")") X("(") X("mp") X("ter") X(" a ") \
	X("ati") X("pe") X("et") X("id") X("nc") X("ie") X(" l") X("rt") \
	X("\n\n") X("B") X("fo") X(" (") X("\n*") X("ly") X("]]\n") X("s a") \
	X("lo") X("ut") X("un") X("uo") X("re ") X("no") X("\n ") X("quo") \
	X("or ") X("uot") X("pr") X("an ") X("ate") X("ho") X("ot;") X("em") \
	X("so") X("&qu") X("&q") X("\n  ") X("M") X("lt") X("P") X("ss") \
	X("pa") X(" re") X("E") X("Th") X("ry") X("im") X(" <") X(">\n") \
	X("  <") X(">\n ") X(" A") X("mo") X("==") X("D") X(" n") X("n t") \
	X("m ") X(" C") X("po") X("ts") X("ad")
#endif

static const unsigned char *dict_codec[] = {
#define X(E) (unsigned char*)E,
DICT_XMACRO
#undef X
};

static const unsigned char dict_sizes[] = {
#define X(E) (sizeof(E) - 1),
DICT_XMACRO
#undef X
};

#define DICT_NELEMS(X) ((sizeof(X)) / sizeof((X)[0]))

#ifndef DICT_MAX_ENTRIES
#define DICT_MAX_ENTRIES (DICT_NELEMS(dict_codec))
#endif

#ifndef DICT_MAX_ENTRY_LENGTH
#define DICT_MAX_ENTRY_LENGTH (3)
#endif

typedef struct {
	int (*get)(void *in);
	int (*put)(void *out, int ch);
	void *in, *out;
} dict_t;

DICT_EXTERN int dict_compress(dict_t *c);
DICT_EXTERN int dict_decompress(dict_t *c);
DICT_EXTERN int dict_tests(void);

#ifdef DICT_IMPLEMENTATION

#define DICT_BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

static int dict_get(dict_t *c) {
	assert(c);
	assert(c->get);
	return c->get(c->in);
}

static int dict_put(dict_t *c, const int ch) {
	assert(c);
	assert(c->put);
	return c->put(c->out, ch);
}

static int dict_gets(dict_t *c, unsigned char *s, int len) {
	assert(s);
	for (int i = 0; i < len; i++) {
		const int ch = dict_get(c);
		if (ch < 0)
			return i;
		s[i] = ch;
	}
	return len;
}

static int dict_puts(dict_t *c, unsigned char *s, int len) {
	assert(s);
	for (int i = 0; i < len; i++) {
		const int ch = dict_put(c, s[i]);
		if (ch < 0)
			return -1;
	}
	return len;
}

static int dict_search(const unsigned char *s, int len) {
	assert(s);
	for (int i = 0; i < (int)DICT_MAX_ENTRIES; i++) {
		const unsigned char size = dict_sizes[i];
		const unsigned char *word = dict_codec[i];
		if (size == len && !memcmp(s, word, size))
			return i;
	}
	return -1;
}

static int dict_find(const unsigned char *s, int len) {
	assert(s);
	for (int i = len; i; i--) {
		const int r = dict_search(s, i);
		if (r >= 0)
			return r;
	}
	return -1;
}

static int dict_flush(dict_t *c, unsigned char *buf, int *length) {
	assert(c);
	assert(buf);
	assert(length);
	const int ibuf = *length;
	assert(ibuf >= 0);
	if (ibuf == 1) {
		if (dict_put(c, 255) < 0)
			return -1;
		if (dict_put(c, buf[0]) < 0)
			return -1;
	} else if (ibuf > 1) {
		if (dict_put(c, 254) < 0)
			return -1;
		if (dict_put(c, ibuf - 2) < 0)
			return -1;
		if (dict_puts(c, buf, ibuf) < 0)
			return -1;
	}
	*length = 0;
	return 0;
}

DICT_API int dict_compress(dict_t *c) {
	assert(c);
	unsigned char buf[257] = { 0, };
	unsigned char word[DICT_MAX_ENTRY_LENGTH] = { 0, };
	int ibuf = 0;
	int len = dict_gets(c, word, DICT_MAX_ENTRY_LENGTH);
	for (;len > 0;) {
		int f = dict_find(word, len);
		if (f < 0) {
			buf[ibuf++] = word[0];
			if (ibuf >= (int)sizeof(buf))
				if (dict_flush(c, buf, &ibuf) < 0)
					return -1;
			memmove(word, word + 1, DICT_MAX_ENTRY_LENGTH - 1);
			const int ch = dict_get(c);
			if (ch < 0)
				len--;
			word[DICT_MAX_ENTRY_LENGTH - 1] = ch;
		} else {
			int size = dict_sizes[f];
			assert(size >= 0);
			assert(size <= DICT_MAX_ENTRY_LENGTH);
			memmove(word, word + size, DICT_MAX_ENTRY_LENGTH - size);
			len -= size;
			int n = dict_gets(c, word + (DICT_MAX_ENTRY_LENGTH - size), size);
			assert(n <= DICT_MAX_ENTRY_LENGTH);
			assert(n >= 0);
			len += n;
			assert(len <= DICT_MAX_ENTRY_LENGTH);
			assert(len >= 0);
			if (dict_flush(c, buf, &ibuf) < 0)
					return -1;
			if (dict_put(c, f) < 0)
				return -1;
		}
	}
	if (dict_flush(c, buf, &ibuf) < 0)
			return -1;
	return 0;
}

DICT_API int dict_decompress(dict_t *c) {
	assert(c);
	for (;;) {
		const int op = dict_get(c);
		if (op < 0)
			return 0;
		if (op == 255) {
			const int lit = dict_get(c);
			if (lit < 0)
				return -1;
			if (dict_put(c, lit) < 0)
				return -1;
		} else if (op == 254) {
			const int cnt = dict_get(c);
			if (cnt < 0)
				return -1;
			for (int i = 0; i < (cnt + 2); i++) {
				const int lit = dict_get(c);
				if (lit < 0)
					return -1;
				if (dict_put(c, lit) < 0)
					return -1;
			}
		} else {
			assert(op >= 0 && op <= (int)DICT_MAX_ENTRIES);
			const unsigned char size = dict_sizes[op];
			const unsigned char *word = dict_codec[op];
			for (int i = 0; i < size; i++) {
				const int lit = word[i];
				if (dict_put(c, lit) < 0)
					return 0;
			}
		}
	}
	return 0;
}

DICT_API int dict_tests(void) {
	DICT_BUILD_BUG_ON(DICT_MAX_ENTRY_LENGTH < 2);
	DICT_BUILD_BUG_ON(DICT_NELEMS(dict_sizes) != DICT_NELEMS(dict_codec));
	DICT_BUILD_BUG_ON(DICT_NELEMS(dict_sizes) > 254);
	return 0;
}
#endif /* DICT_IMPLEMENTATION */
#endif
