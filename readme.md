# DICT

* Project: A hard-coded dictionary based compression library
* Author: Richard James Howe
* License: 0BSD / Public Domain
* Email: <mailto:howe.r.j.89@gmail.com>
* Repo: <https://github.com/howerj/dict>

This is a hard-coded dictionary based compression library similar to
<https://github.com/antirez/smaz>. It was rolled out very quickly.

The table of entries was formed using a utility of mine called *ngram*,
available here <https://github.com/howerj/ngram>. Given a corpus of text
(I use the term "text" here loosely, binary data should work as well),
we can create a dictionary suited to compressing that data.

The dictionary is specified as an [X-Macro](https://en.wikipedia.org/wiki/X_macro)
and is structured as a 
[Header-only library](https://en.wikipedia.org/wiki/Header-only), and as
such it should be fairly easy to incorporate into a project. You can
view the file [main.c][] to see an example usage.

The library needs some optimization, it does not perform as well as *smaz*,
although currently it only needs a single dictionary for both compression
and decompression (a table of lengths is generated at compile time), whereas 
*smaz* needs one for each. The table is also easier to replace. The I/O
mechanism in this library is also more generic.

*smaz* uses a hash table and allows for multi-byte literal data, this library
uses linear search and only allows for single-byte literal data. Decoding is
fast but encoding is very slow. There are lots of things that could be used
to improve the library, however it was just thrown together really quickly
without any thought. Perhaps later versions will correct this, even a simple
binary search should speed things up.

Worst case performance doubles the input. 

`alt.h` contains an alternative dictionary, which can be optionally compiled
in, replacing the default dictionary. `alt.h` has been trained on C programs,
whilst the built in dictionary has been trained on [enwik8](https://en.wikipedia.org/wiki/Hutter_Prize),
using the mentioned ngram utility.

## USAGE

The example program, in `main.c`, requires a C compiler. It acts as
a Unix filter, usage is as so:

	./dict c|d < infile > outfile

* `c` for compression.
* `d` for decompression.

## RETURN CODE

This program returns non-zero on failure.


