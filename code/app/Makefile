top_builddir = ../..
include ${top_builddir}/makeinclude

all:
	${MAKE} -C src dep
	${MAKE} -C src obj
	${MAKE} -C src ihx

clean:
	${MAKE} -C src clean
	rm -rf *~ */*~
