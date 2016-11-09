GEDA_SCHEME_DIR = /usr/share/gEDA/scheme
QUAZAR_GEDA_DIR = $(HOME)/gEDA

SCHEMATICS = \
1-power.sch 2-mcu.sch 3-adc.sch

PS_FILES = $(SCHEMATICS:%.sch=%.ps)
PDF_FILES = $(SCHEMATICS:%.sch=%.pdf)

PROJECT_NAME = DAQ_PHY315

all:
	@echo "Specify make target -- pdf, bom, or  clean."

pdf: schematic.pdf

schematic.pdf: $(PDF_FILES)
	gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=$@ $(PDF_FILES)

%.ps:%.sch
	gschem -p -o$@ -s$(GEDA_SCHEME_DIR)/print.scm $<

%.pdf: %.ps
	ps2pdf $<

bom: bom.csv

bom.csv: $(SCHEMATICS) $(QUAZAR_GEDA_DIR)/pricelist.csv
	gnetlist -g partslist3 -o bom.csv $(SCHEMATICS)
	bom2bom \
	    --mode production \
	    --input bom.csv \
	    --pricelist $(QUAZAR_GEDA_DIR)/pricelist.csv \
	    --exclude-device TESTPOINT \
	    --exclude-device "SOLDER DOT" \
	    --output bom.csv \
	    --project-name "$(PROJECT_NAME)" \
	    --texfile bom.tex  
	pdflatex bom.tex 
	pdflatex bom.tex 
	pdflatex bom.tex 

clean:
	rm -vf $(PS_FILES) $(PDF_FILES) *.tex *.aux *.log *~

.PHONY: pdf bom clean
