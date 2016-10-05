top_builddir = ../../..
include $(top_builddir)/makeinclude

SRC = \
	SPI.cxx \
	I2C.cxx \
	UART.cxx

DEP = $(SRC:%.cxx=%.dep)
ASM = $(SRC:%.cxx=%.s)
OBJ = $(SRC:%.cxx=%.o)

ifeq ($(MAKECMDGOALS), obj)
	include $(DEP)
endif

dep: $(DEP)

obj: $(OBJ)

lib: libavr.a

libavr.a: $(OBJ)
	$(AR) $@ $(OBJ)

clean:
	$(RM) $(DEP) $(OBJ) $(ASM) libavr.a *~

.PHONY: dep obj lib clean
