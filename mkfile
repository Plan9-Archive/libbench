</$objtype/mkfile

P=bench

LIB=lib$P.$O.a
OFILES=$P.$O
HFILES=/sys/include/$P.h

</sys/src/cmd/mksyslib

install:V:	$LIB
	cp $LIB /$objtype/lib/lib$P.a
	cp $P.h /sys/include/$P.h

uninstall:V:
	rm -f /$objtype/lib/lib$P.a /sys/include/$P.h

nuke:V:
	mk uninstall

clean:V:
	@{ cd benchmarks; mk $target }
	rm -f *.[$OS] [$OS].out $LIB

benchmark:QV:
	@{
		cd benchmarks
		mk all
		for(t in $O.benchmark_*){
			echo $t...
			./$t
		}
	}

benchmark.%:QV:
	@{
		cd benchmarks
		mk $O.benchmark_$stem
		./$O.benchmark_$stem
	}
