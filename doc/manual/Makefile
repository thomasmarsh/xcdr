FILE=concepts_guide
DOT_SOURCES = test.dot
PS_SOURCES = test.ps $(DOT_SOURCES:.dot=.ps)
EPS_SOURCES = $(PS_SOURCES:.ps=.eps)
PDF_SOURCES = $(EPS_SOURCES:.eps=.pdf)


DENSITY=300x300
GEOMETRY=80%x80%
POINTSIZE=12

CONVERT = convert \
	-pointsize $(POINTSIZE) \
	-geometry $(GEOMETRY) \
	-density $(DENSITY) \
	-compress rle

TEX=$(FILE).tex

DVI=$(FILE).dvi
PDF=$(FILE).pdf

LATEX=latex

INCLUDES = \
	title.tex \
	preface.tex \
	highlevel.tex \
	collection.tex \
	algorithm2e.sty \
	fancyhea.sty \
	xcdr.sty

all: dvi

dvi: $(DVI)
pdf: $(PDF)

$(DVI): $(TEX) $(EPS_SOURCES) $(INCLUDES)
	@echo "--------------------------------------------------------------------"
	@echo " BUILD DVI"
	@echo "--------------------------------------------------------------------"
	latex $<
	makeindex $(FILE).idx
#	cat concepts_guide.loa | sed -e 's/theHalgo/thealgo/g'
	latex $<

$(PDF): $(TEX) $(PDF_SOURCES) $(INCLUDES)
	@echo "--------------------------------------------------------------------"
	@echo " BUILD PDF"
	@echo "--------------------------------------------------------------------"
	pdflatex $<
	pdflatex $<

%.ps: %.dot
	dot -Tps $< > $@

%.eps: %.ps
	$(CONVERT) $< eps:$@

%.pdf: %.eps
	epstopdf $<

clean:
	rm -f *.pdf *.dvi *.toc *.aux *.log *.loa *.lof *.idx *.ind *.lot
