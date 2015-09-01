#include <u.h>
#include <libc.h>
#include <mp.h>
#include <libsec.h>
#include <bench.h>

void
benchrand(B *b)
{
	int i;

	for(i = 0; i < b->N; i++)
		rand();

	benchbytes(b, sizeof(int));
}

void
benchtruerand(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		(void)truerand();
	}

	benchbytes(b, sizeof(ulong));
}

void
benchfastrand(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		(void)fastrand();
	}

	benchbytes(b, sizeof(ulong));
}

BItem tests[] = {
	{"rand", benchrand},
	{"truerand", benchtruerand},
	{"fastrand", benchfastrand},
};

void
main(int argc, char *argv[])
{
	int i;

	ARGBEGIN{
	}ARGEND

	/* open files, seed, etc */
	truerand();
	fastrand();

	benchinit();

	if(argv[0] == nil){
		for(i = 0; i < nelem(tests); i++){
			bench(tests[i].name, tests[i].fn);
		}
	} else {
		for(i = 0; i < nelem(tests); i++){
			if(strncmp(argv[0], tests[i].name, strlen(argv[0])) == 0)
				bench(tests[i].name, tests[i].fn);
		}
	}

	exits(0);
}
