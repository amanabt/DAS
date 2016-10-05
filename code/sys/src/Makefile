top_builddir = ../../..
include $(top_builddir)/makeinclude

SRC = \
 	Applet.cxx \
 	QP4.cxx \
# 	Timer.cxx \

DEP = $(SRC:%.cxx=%.dep)
ASM = $(SRC:%.cxx=%.s)
OBJ = $(SRC:%.cxx=%.o)

ifeq ($(MAKECMDGOALS), obj)
	include $(DEP)
endif

dep: $(DEP)

obj: $(OBJ)

lib: libsys.a

libsys.a: $(OBJ)
	$(AR) $@ $(OBJ)

clean:
	$(RM) $(DEP) $(OBJ) $(ASM) libsys.a *~

.PHONY: dep obj lib clean
