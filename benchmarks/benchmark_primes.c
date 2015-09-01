#include <u.h>
#include <libc.h>
#include <bench.h>
#include <mp.h>
#include <libsec.h>

int primebits;

void
benchgenprime(B *b)
{
	int i;
	mpint *p;

	p = mpnew(primebits);

	for(i = 0; i < b->N; i++) {
		genprime(p, primebits, 0);
	}
}

void
main(int argc, char *argv[])
{
	ARGBEGIN{
	}ARGEND

	benchinit();

	primebits = 256;
	bench("genprime 256", benchgenprime);

	primebits = 2048;
	bench("genprime 2048", benchgenprime);

	exits(nil);
}
