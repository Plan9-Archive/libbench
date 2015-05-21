#include <u.h>
#include <libc.h>
#include <bench.h>

void
benchmallocfree(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		free(malloc(i));
	}
}

void
benchrand(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		(void)rand();
	}
}

void
benchtruerand(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		(void)truerand();
	}
}

void
benchinc(B *b)
{
	int i;
	long inc;

	for(i = 0; i < b->N; i++) {
		inc++;
	}
}

void
benchainc(B *b)
{
	int i;
	long inc;

	for(i = 0; i < b->N; i++) {
		ainc(&inc);
	}
}

void
benchopenclose(B *b)
{
	int i, fd;
	for(i = 0; i < b->N; i++){
		fd = open("/dev/zero", OREAD);
		close(fd);
	}
}

void
benchfork(B *b)
{
	int i;
	
	for(i = 0; i < b->N; i++){
		if(!fork())
			exits(nil);
		waitpid();
	}
}

void
benchforkexecl(B *b)
{
	int i;
	
	for(i = 0; i < b->N; i++){
		if(!fork()){
			execl("./true", nil);
			fprint(2, "execl: %r\n");
			exits("execl");
		}
		waitpid();
	}
}

struct {
	char *name;
	BFn fun;
} tests[] = {
	{"mallocfree", benchmallocfree},
	{"rand", benchrand},
	{"truerand", benchtruerand},
	{"inc", benchinc},
	{"ainc", benchainc},
	{"openclose", benchopenclose},
	{"fork", benchfork},
	{"forkexecl", benchforkexecl},
};

void
main(int argc, char *argv[])
{
	int i;

	ARGBEGIN{
	}ARGEND

	benchinit();

	if(argv[0] == nil){
		for(i = 0; i < nelem(tests); i++){
			bench(tests[i].name, tests[i].fun);
		}
	} else {
		for(i = 0; i < nelem(tests); i++){
			if(strncmp(argv[0], tests[i].name, strlen(argv[0])) == 0)
				bench(tests[i].name, tests[i].fun);
		}
	}

	exits(0);
}

