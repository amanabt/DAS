top_builddir = ../../..
include $(top_builddir)/makeinclude

CPP_SRC = \
	QP4.cxx \
	FTDI.cxx

OBJ  = $(CPP_SRC:%.cxx=%.o)
LOBJ = $(CPP_SRC:%.cxx=%.lo)
DEP  = $(CPP_SRC:%.cxx=%.dep)

ifeq ($(MAKECMDGOALS), obj)
include $(DEP)
endif

dep: $(DEP)

obj: $(LOBJ)

lib: libsys.la

libsys.la: $(LOBJ)
	$(LINK) $(LOBJ) -o $@

clean:
	$(RM) $(DEP) $(OBJ) $(LOBJ) libsys.la .libs *~

.PHONY: dep obj lib clean
