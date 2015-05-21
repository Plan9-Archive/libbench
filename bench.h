#pragma lib	"libbench.a"
#pragma src	"/sys/src/libbench"

typedef struct BItem BItem;
typedef struct BResult BResult;
typedef struct B B;

typedef void (*BFn)(B* b);

// single benchmark function
struct BItem
{
	char *name;
	BFn fn;
};

// result of benchmarking
struct BResult
{
	int N;
	vlong ns;
	uvlong cycles;
};

// type passed to bench functions
struct B
{
	int N;
	vlong start;	/* start ns */
	vlong ns;	/* duration */
	uvlong scycles;	/* start cycles */
	uvlong ecycles;	/* end cycles */
	uvlong bcycles;	/* best cycles */
	BItem item;
};

// public api
void benchinit(void);
void bench(char *name, BFn);
void benchitems(BItem[], int);