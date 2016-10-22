top_builddir = ../../..
include ${top_builddir}/makeinclude

CPP_SRC = \
	Comm.cxx \
	Driver.cxx

DEP  = ${CPP_SRC:%.cxx=%.dep}
OBJ  = ${CPP_SRC:%.cxx=%.o}
LOBJ = ${CPP_SRC:%.cxx=%.lo}

ifeq (${MAKECMDGOALS}, obj)
include $(DEP)
endif

dep: $(DEP)

obj: $(LOBJ)

lib: libdriver.la libDAS.a

libdriver.la: $(LOBJ)
	$(LINK) $(LOBJ) -o $@

libDAS.a:
	$(LINK) $(LIBS) -o $@

clean:
	$(RM) $(DEP) $(OBJ) $(LOBJ) libdriver.la libDAS.a .libs test main *~

.PHONY: dep obj lib clean
