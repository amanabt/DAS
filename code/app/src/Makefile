top_builddir = ../../..
include $(top_builddir)/makeinclude

SRC = \
	Comm.cxx \
	Application.cxx \

DEP = $(SRC:%.cxx=%.dep)
ASM = $(SRC:%.cxx=%.s)
OBJ = $(SRC:%.cxx=%.o)

ifeq ($(MAKECMDGOALS), obj)
	include $(DEP)
endif

dep: $(DEP)

obj: $(OBJ)

ihx: app.ihx

app.elf: $(OBJ) $(LIBDEPS)
	$(CC) $(LFLAGS) $(OBJ) $(LIBS) -o $@

app.ihx: app.elf
	$(OBJCOPY) -O ihex $< $@

clean:
	$(RM) $(DEP) $(OBJ) $(ASM) app.elf app.map *~

.PHONY: dep obj clean ihx
