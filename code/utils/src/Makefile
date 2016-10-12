top_builddir = ../../..
include $(top_builddir)/makeinclude

SRC =

DEP  = $(SRC:%.cxx=%.dep)
OBJ  = $(SRC:%.cxx=%.o)
LOBJ = $(SRC:%.cxx=%.lo)

ifeq ($(MAKECMDGOALS), obj)
	include $(DEP)
endif

dep: $(DEP)

obj: $(LOBJ)

lib: libutils.la

libutils.la: $(LOBJ)
	$(LINK) $(LOBJ) -o $@

clean:
	$(RM) $(DEP) $(OBJ) $(LOBJ) libutils.la .libs *~

.PHONY: dep obj lib clean
