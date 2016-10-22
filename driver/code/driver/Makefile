top_builddir = ../..
include ${top_builddir}/makeinclude

all:
	${MAKE} -C src dep
	${MAKE} -C src obj
	${MAKE} -C src lib

install: all
	mkdir -p $(INSTALL_INCLUDE_DIR)/driver
	cp -r ./include $(INSTALL_INCLUDE_DIR)/driver

	mkdir -p $(INSTALL_LIB_DIR)
	cp -r ./src/libDAS.a $(INSTALL_LIB_DIR)

clean:
	${MAKE} -C src clean
	rm -rf *~ */*~
