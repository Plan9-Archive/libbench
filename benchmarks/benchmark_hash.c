#include <u.h>
#include <libc.h>
#include <bench.h>
#include <libsec.h>

DigestState *(*hashfn)(uchar *data, ulong dlen, uchar *digest, DigestState *state);
DigestState *(*hmacfn)(uchar *data, ulong dlen, uchar *key, ulong klen, uchar *digest, DigestState *state);

int i;
uchar glass[8192];
uchar digest[SHA2_512dlen];

uchar key[16] = "I can eat glass";

void
benchhashfn(B *b)
{
	for(i = 0; i < b->N; i++){
		hashfn(glass, sizeof glass, digest, nil);
	}
	benchbytes(b, sizeof glass);
}

void
benchhmacfn(B *b)
{
	for(i = 0; i < b->N; i++){
		hmacfn(glass, sizeof glass, key, 16, digest, nil);
	}
	benchbytes(b, sizeof glass);
}

void
loadglass(void)
{
	int fd;

	fd = open("/lib/glass", OREAD);
	if(fd < 0)
		sysfatal("open: %r");

	if(read(fd, glass, sizeof(glass)) != sizeof(glass))
		sysfatal("read: %r");

	close(fd);
}

void
main(int argc, char *argv[])
{
	ARGBEGIN{
	}ARGEND

	loadglass();

	benchinit();

	hashfn = md5;
	bench("md5", benchhashfn);

	hashfn = sha1;
	bench("sha1", benchhashfn);

	hashfn = sha2_224;
	bench("sha224", benchhashfn);

	hashfn = sha2_256;
	bench("sha256", benchhashfn);

	hashfn = sha2_384;
	bench("sha384", benchhashfn);

	hashfn = sha2_512;
	bench("sha512", benchhashfn);

	hashfn = ripemd160;
	bench("ripemd160", benchhashfn);

	hmacfn = hmac_md5;
	bench("hmac(md5)", benchhmacfn);

	hmacfn = hmac_sha1;
	bench("hmac(sha1)", benchhmacfn);

	hmacfn = hmac_sha2_224;
	bench("hmac(sha224)", benchhmacfn);

	hmacfn = hmac_sha2_256;
	bench("hmac(sha256)", benchhmacfn);

	hmacfn = hmac_sha2_384;
	bench("hmac(sha384)", benchhmacfn);

	hmacfn = hmac_sha2_512;
	bench("hmac(sha512)", benchhmacfn);

	exits(nil);
}
