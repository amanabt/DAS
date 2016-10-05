top_builddir = .
include ${top_builddir}/makeinclude

all:
	@echo "Please specify target: ihx, doc, burn, or clean."

ihx:
	${MAKE} -C code all

doc:
	doxygen doxy.conf

burn:
	$(AVRDUDE) -p atmega328 -P ft0 -B 200 \
		-c USBasp -U flash:w:code/app/src/app.ihx
#	$(AVRDUDE) -p atmega328 -P /dev/ttyUSB0 \
#		-c stk500v2 -b 38400 -U flash:w:code/app/src/app.ihx

clean:
	${MAKE} -C code clean
	rm -f *~

.PHONY: all bin doc burn clean

