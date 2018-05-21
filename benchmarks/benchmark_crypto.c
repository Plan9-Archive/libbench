#include <u.h>
#include <libc.h>
#include <bench.h>
#include <mp.h>
#include <libsec.h>

DigestState *(*hashfn)(uchar *data, ulong dlen, uchar *digest, DigestState *state);
DigestState *(*hmacfn)(uchar *data, ulong dlen, uchar *key, ulong klen, uchar *digest, DigestState *state);

int i;
uchar block[8192];
uchar digest[SHA2_512dlen];

uchar key[40] = "I can eat glass and it doesn't hurt me.";

int keybytes;

uchar randombuf[1024];

void
benchaescbc(B *b)
{
	int i;
	AESstate s;

	setupAESstate(&s, key, keybytes, key);

	benchreset(b);

	for(i = 0; i < b->N; i++) {
		aesCBCencrypt(block, sizeof block, &s);
	}

	benchbytes(b, sizeof block);
}

void
benchbfcbc(B *b)
{
	int i;
	BFstate s;

	setupBFstate(&s, key, keybytes, nil);

	benchreset(b);

	for(i = 0; i < b->N; i++) {
		bfCBCencrypt(block, sizeof block, &s);
	}

	benchbytes(b, sizeof block);
}

void
benchrsagen2048(B *b)
{
	int i;
	RSApriv *p;

	for(i = 0; i < b->N; i++) {
		p = rsagen(2048, 65537, 0);
		assert(p);
		rsaprivfree(p);
	}
}

void
benchdsagen(B *b)
{
	int i;
	DSApriv *p;

	for(i = 0; i < b->N; i++) {
		p = dsagen(nil);
		assert(p);
		dsaprivfree(p);
	}
}

void
ecprivfree(ECpriv *priv)
{
	if(priv != nil) {
		mpfree(priv->x);
		mpfree(priv->y);
		mpfree(priv->d);
	}

	free(priv);
}

void
benchecgen(B *b)
{
	int i;
	ECdomain *dom;
	ECpriv *p;

	dom = mallocz(sizeof(*dom), 1);
	ecdominit(dom, secp256r1);
	assert(dom);

	benchreset(b);

	for(i = 0; i < b->N; i++) {
		p = ecgen(dom, nil);
		assert(p);
		ecprivfree(p);
	}

	ecdomfree(dom);
}

void
benchcurve25519(B *b)
{
	int i;

	for(i = 0; i < b->N; i++) {
		curve25519(randombuf, randombuf+32, randombuf+64);
	}
}

void
main(int argc, char *argv[])
{
	ARGBEGIN{
	}ARGEND

	genrandom(randombuf, sizeof randombuf);

	benchinit();

	keybytes = 16;
	bench("aes-cbc 16", benchaescbc);
	keybytes = 24;
	bench("aes-cbc 24", benchaescbc);
	keybytes = 32;
	bench("aes-cbc 32", benchaescbc);

	keybytes = 16;
	bench("blowfish-cbc 16", benchbfcbc);
	keybytes = 24;
	bench("blowfish-cbc 24", benchbfcbc);
	keybytes = 32;
	bench("blowfish-cbc 32", benchbfcbc);

	bench("rsagen 2048", benchrsagen2048);

	bench("dsagen", benchdsagen);

	bench("ecgen", benchecgen);

	bench("curve25519", benchcurve25519);

	exits(nil);
}
