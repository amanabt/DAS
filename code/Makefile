top_builddir = ..
include ${top_builddir}/makeinclude

DIRS = sys utils driver

all:
	for dir in ${DIRS}; do \
		${MAKE} -C $${dir} all; \
	done

install: install_stl
	for dir in ${DIRS}; do \
		${MAKE} -C $${dir} install; \
	done

install_stl:
	mkdir -p $(INSTALL_INCLUDE_DIR)
	cp -r ./stl $(INSTALL_INCLUDE_DIR)

clean:
	for dir in ${DIRS}; do \
		${MAKE} -C $${dir} clean; \
	done
	rm -rf *~

.PHONY: all clean
