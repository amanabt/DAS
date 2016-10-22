top_builddir = .
include ${top_builddir}/makeinclude

CXXS = test.cxx

OBJS = ${CXXS:%.cxx=%.o}

all: test

test: ${OBJS}
	${CC} ${OBJS} ${LIBS} -o $@

clean:
	rm -f ${OBJS} test

.PHONY: clean
