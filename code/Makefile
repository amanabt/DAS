top_builddir = ..
include ${top_builddir}/makeinclude

all:
	${MAKE} -C avr     all
	${MAKE} -C sys     all
	${MAKE} -C app     all

clean:
	${MAKE} -C avr     clean
	${MAKE} -C sys     clean
	${MAKE} -C app     clean
	rm -rf *~

.PHONY: all clean

