</$objtype/mkfile

P=theme

LIB=lib$P.$O.a
OFILES=$P.$O
HFILES=/sys/include/$P.h

</sys/src/cmd/mklib

install:V:	$LIB
	cp $LIB /$objtype/lib/lib$P.a
	cp $P.h /sys/include/$P.h
	cp $P.man2 /sys/man/2/$P

uninstall:V:
	rm -f /$objtype/lib/lib$P.a /sys/include/$P.h /sys/man/2/$P

clean:V:
	rm -f *.[$OS] [$OS].* $LIB

$O.%:	%.$O $OFILES $LIB $TESTLIB
	$LD $LDFLAGS -o $target $prereq

test:VQ:
	mk $O.theme_test
	echo $O.theme_test
	$O.theme_test
