</$objtype/mkfile

P=bench

LIB=lib$P.$O.a
OFILES=$P.$O
HFILES=/sys/include/$P.h
CLEANFILES=$O.test $O.true true $O.crypto

</sys/src/cmd/mksyslib

install:V:	$LIB
	cp $LIB /$objtype/lib/lib$P.a
	cp $P.h /sys/include/$P.h

uninstall:V:
	rm -f /$objtype/lib/lib$P.a /sys/include/$P.h

test:V:	$O.test true
	$O.test

$O.test:	test.$O
	$LD -o $target $prereq

true: $O.true
	cp $O.true true

$O.true:	true.$O
	$LD -o $target $prereq

crypto:V:	$O.crypto
	$O.crypto

$O.crypto:	crypto.$O
	$LD -o $target $prereq
