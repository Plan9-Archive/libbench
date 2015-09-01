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

/* get rid of this goo once libsec has better ec functions */
void
_ecfreepoint(ECpoint *pt)
{
	if(pt != nil) {
		mpfree(pt->x);
		mpfree(pt->y);
	}

	free(pt);
}

void
_ecfreedomain(ECdomain *dom)
{
	if(dom != nil) {
		mpfree(dom->p);
		mpfree(dom->a);
		mpfree(dom->b);
		if(dom->G != nil) {
			_ecfreepoint(dom->G);
		}
		mpfree(dom->n);
		mpfree(dom->h);
	}

	free(dom);
}

ECdomain *
_ecnamedcurve(int id)
{
	ECdomain *dom;
	dom = malloc(sizeof(ECdomain));
	if(dom == nil)
		return nil;

	switch(id) {
	default:
		free(dom);
		return nil;
	/* Secp256r1 */
	case 0:
		dom->p = strtomp("FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF", nil, 16, nil);
		dom->a = strtomp("FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC", nil, 16, nil);
		dom->b = strtomp("5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B", nil, 16, nil);
		dom->G = strtoec(dom, "036B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296", nil, nil);
		dom->n = strtomp("FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551", nil, 16, nil);
		dom->h = uitomp(1, nil);
		break;
	}

	if(dom->p == nil || dom->a == nil || dom->b == nil || dom->G == nil || dom->n == nil || dom->h == nil) {
		_ecfreedomain(dom);
		return nil;
	}

	return dom;
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

	dom = _ecnamedcurve(0);
	assert(dom);

	benchreset(b);

	for(i = 0; i < b->N; i++) {
		p = ecgen(dom, nil);
		assert(p);
		ecprivfree(p);
	}

	_ecfreedomain(dom);
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
