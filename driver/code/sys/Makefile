top_builddir = ../..
include ${top_builddir}/makeinclude

all:
	${MAKE} -C src dep
	${MAKE} -C src obj
	${MAKE} -C src lib

install:
	mkdir -p $(INSTALL_INCLUDE_DIR)/sys
	cp -r ./include $(INSTALL_INCLUDE_DIR)/sys

clean:
	${MAKE} -C src clean
	rm -rf *~ */*~
