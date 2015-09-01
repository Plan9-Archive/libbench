#include <u.h>
#include <libc.h>
#include <bench.h>

#define BENCHTIME (1000000000)	/* 1s in ns */

uvlong boverhead;

static int
min(int x, int y)
{
        if(x > y) {
                return y;
        }
        return x;
}

static int
max(int x, int y)
{
        if(x < y) {
                return y;
        }
        return x;
}

// run the benchmarking function once, looping n times
static void
benchrunn(B *b, int n)
{

	b->N = n;

	// reset
	b->start = nsec();
	b->ns = 0;
	cycles(&b->scycles);

	b->item.fn(b);

	// stop
	cycles(&b->ecycles);
	b->ns += nsec() - b->start;
	b->bcycles += b->ecycles - b->scycles - boverhead;
}

static vlong
nsperop(B *b)
{
	if(b->N <= 0)
		return 0;

	return b->ns / (vlong)b->N;
}

static uvlong
cyperop(B *b)
{
	if(b->N <= 0)
		return 0;

	return b->bcycles / (uvlong)b->N;
}

static int
rounddown10(int n)
{
	int tens, result, i;

	tens = 0;

	while(n >= 10) {
		n = n / 10;
		tens++;
	}

	result = 1;

	for(i = 0; i < tens; i++) {
		result *= 10;
	}

	return result;
}

static int
roundup(int n)
{
	int base;

	base = rounddown10(n);

	if(n <= base)
		return base;
	if(n <= 2*base)
		return 2*base;
	if(n <= 5*base)
		return 5*base;

	return 10*base;
}

// run the benchmark for one function
static BResult
benchrun(B *b)
{
	int n, last;
	vlong d;
	BResult res;

	n = 1;

	benchrunn(b, n);

	d = BENCHTIME;

	while(b->ns < d && n < 1000000000) {
		last = n;
		if(nsperop(b) == 0) {
			n = 1000000000;
		} else {
			n = (int) d/nsperop(b);
		}

		n = max(min(n+n/2, 100*last), last+1);

		n = roundup(n);
		benchrunn(b, n);
	}

	res.N = b->N;
	res.ns = b->ns;
	res.cycles = b->bcycles;
	res.bytes = b->bytes;

	return res;
}

static void
benchres(BResult *res)
{
	char nsop[32];
	char cyop[32];
	vlong nsperop;
	uvlong cyperop;
	double mbps;

	if(res->N <= 0) {
		nsperop = 0;
		cyperop = 0;
	} else {
		nsperop = res->ns / (vlong)res->N;
		cyperop = res->cycles / (uvlong)res->N;
	}

	snprint(nsop, sizeof(nsop), "%10lld    ns/op", nsperop);
	snprint(cyop, sizeof(cyop), "%10ulld    cy/op", cyperop);

	if(res->N > 0 && nsperop < 100) {
		if(nsperop < 10)
			snprint(nsop, sizeof(nsop), "%13.2f ns/op", (double)res->ns / (double)res->N);
		else
			snprint(nsop, sizeof(nsop), "%12.1f  ns/op", (double)res->ns / (double)res->N);
	}

	if(res->N > 0 && cyperop < 100) {
		if(cyperop < 10)
			snprint(cyop, sizeof(cyop), "%13.2f cy/op", (double)res->cycles / (double)res->N);
		else
			snprint(cyop, sizeof(cyop), "%12.1f  cy/op", (double)res->cycles / (double)res->N);
	}

	print("%10d N\t%s\t%s", res->N, nsop, cyop);

	/* show MB/s if test called benchbytes */
	if(res->bytes > 0){
		mbps = ((double)res->bytes * (double)res->N / 1000000.0) / ((double)res->ns / 1000000000.0);
		print("\t%7.2f MB/s", mbps);
	}

	print("\n");
}

/*
 * public api
*/

// setup. currently only calculates cycles() overhead.
// not strictly necessary, but will give better cycle counts.
void
benchinit(void)
{
	int at;
	uvlong a, b;

	/* figure out cycles overhead */
	boverhead = -1;

	for(at = 0; at < 1000000; at++) {
		cycles(&a);
		cycles(&b);
		if(boverhead > b - a)
			boverhead = b - a;
	}

}

// bench a single function
void
bench(char *name, BFn fn)
{
	B b;
	BResult res;

	memset(&b, 0, sizeof(B));
	memset(&res, 0, sizeof(BResult));

	b.item.name = name;
	b.item.fn = fn;

	print("%16s\t", name);
	res = benchrun(&b);

	benchres(&res);
}

// bench an array of functions
void
benchitems(BItem items[], int len)
{
	int i;

	for(i = 0; i < len; i++) {
		bench(items[i].name, items[i].fn);
	}
}

/* benchbytes records the number of bytes processed in a single operation. */
void
benchbytes(B *b, vlong bytes)
{
	b->bytes = bytes;
}
