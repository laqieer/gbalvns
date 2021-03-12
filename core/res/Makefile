BINFILES	= $(wildcard *.bin)
TXTFILES	= $(wildcard *.txt)
BMPFILES	= $(wildcard *.bmp)
PNGFILES	= $(wildcard *.png)
HFILES	= $(wildcard *.h)

SFILES	= \
	$(BINFILES:.bin=.s) \
	$(TXTFILES:.txt=.s) \
	$(BMPFILES:.bmp=.s) \
	$(PNGFILES:.png=.s)

CFILES	= 

#=============================================================================
all: $(SFILES) $(CFILES) ;

clean:
	rm -f $(SFILES) $(HFILES)

.SUFFIXES: .S .s .c .o .a .d .bmp .png .bin .txt

.bin.s: $(BINFILES)
	@echo \# converting $<
	@bin2s $< > $@

.txt.s: $(TXTFILES)
	@echo \# converting $<
	@bin2s $< > $@

.bmp.s: $(BMPFILES)
	@echo \# converting $<
	@grit $< -ff$*.grit

.png.s: $(PNGFILES)
	@echo \# converting $<
	@grit $< -ff$*.grit
