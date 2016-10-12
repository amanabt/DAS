top_builddir = ../..
include ${top_builddir}/makeinclude

all:
	${MAKE} -C src dep
	${MAKE} -C src obj
	${MAKE} -C src lib

install:
	mkdir -p $(INSTALL_INCLUDE_DIR)/utils
	cp -r ./include $(INSTALL_INCLUDE_DIR)/utils

clean:
	${MAKE} -C src clean
	rm -rf *~ */*~
