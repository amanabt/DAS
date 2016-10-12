top_builddir = .
include ${top_builddir}/makeinclude

all:
	@echo "Please specify target: " \
	"lib, install, uninstall, doc, checkin, or clean."

lib:
	${MAKE} -C code all

install: install_lib install_udev_rules

install_lib: lib
	${MAKE} -C code install

install_udev_rules:
	@if [ ! -f $(UDEV_RULES_FOLDER)/99-libftdi.rules ]; then \
		touch $(UDEV_RULES_FOLDER)/99-libftdi.rules; \
	fi

	@cat $(UDEV_RULES_FOLDER)/99-libftdi.rules | \
		grep $(subst ",\",ATTRS{idVendor}=="0403") | \
		grep -q $(subst ",\",ATTRS{idProduct}=="6001"); \
	status=$$?; \
	if [ $$status != 0 ]; then \
		cat $(top_builddir)/resources/99-libftdi.rules >> \
			$(UDEV_RULES_FOLDER)/99-libftdi.rules; \
	fi

uninstall:
	rm -rf \
		$(INSTALL_INCLUDE_DIR) \
		$(INSTALL_LIB_DIR)/libDAS.a

doc:
	doxygen doxy.conf

clean:
	rm -f *~
	rm -rf doc
	${MAKE} -C code clean

.PHONY: all lib install install_lib install_udev_rules doc clean
