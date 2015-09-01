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
benchinc(B *b)
{
	int i;
	long inc = 0;

	for(i = 0; i < b->N; i++) {
		inc++;
	}
}

void
benchainc(B *b)
{
	int i;
	long inc = 0;

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
			execl("/bin/syscall", "syscall", "exits", nil);
			fprint(2, "execl: %r\n");
			exits("execl");
		}
		waitpid();
	}
}

void
benchmemcpy(B *b)
{
	int i;
	uchar mem[2048];
	
	for(i = 0; i < b->N; i++){
		memcpy(mem, mem+1024, 1024);
	}

	benchbytes(b, 1024);
}

void
benchmemmove(B *b)
{
	int i;
	uchar mem[2048];
	
	for(i = 0; i < b->N; i++){
		memmove(mem, mem+1024, 1024);
	}

	benchbytes(b, 1024);
}

BItem tests[] = {
	{"mallocfree", benchmallocfree},
	{"inc", benchinc},
	{"ainc", benchainc},
	{"openclose", benchopenclose},
	{"fork", benchfork},
	{"forkexecl", benchforkexecl},
	{"memcpy", benchmemcpy},
	{"memmove", benchmemmove},
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
