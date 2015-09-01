#include <u.h>
#include <libc.h>
#include <mp.h>
#include <libsec.h>
#include <bench.h>

uchar in[1024];
char out[2048];

void
benchenc16(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		enc16(out, sizeof out, in, sizeof in);
	}

	benchbytes(b, sizeof in);
}

void
benchdec16(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		dec16(in, sizeof in, out, sizeof out);
	}

	benchbytes(b, sizeof out);
}

void
benchenc64(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		enc64(out, sizeof out, in, sizeof in);
	}

	benchbytes(b, sizeof in);
}

void
benchdec64(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		dec64(in, sizeof in, out, sizeof out);
	}

	benchbytes(b, sizeof out);
}

void
main(int argc, char *argv[])
{
	ARGBEGIN{
	}ARGEND

	genrandom(in, sizeof in);

	benchinit();

	bench("enc16", benchenc16);
	bench("dec16", benchdec16);
	bench("enc64", benchenc64);
	bench("dec64", benchdec64);
	exits(nil);
}
