</$objtype/mkfile

P=bench

LIB=lib$P.$O.a
OFILES=$P.$O
HFILES=/sys/include/$P.h
CLEANFILES=$O.test $O.true true

</sys/src/cmd/mksyslib

install:V:	$LIB
	cp $LIB /$objtype/lib/lib$P.a
	cp $P.h /sys/include/$P.h

uninstall:V:
	rm -f /$objtype/lib/lib$P.a /sys/include/$P.h

test:	$O.test true
	$O.test

$O.test:	test.$O $LIB
	$LD -o $target $prereq

true: $O.true
	cp $O.true true

$O.true:	true.$O
	$LD -o $target $prereq
