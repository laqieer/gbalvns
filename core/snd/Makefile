WAVFILES	= $(wildcard *.wav)

SFILES	= \
	$(WAVFILES:.wav=.s)

CFILES	= 

#=============================================================================
all: $(SFILES) $(CFILES) ;

clean:
	rm -f $(SFILES)

.SUFFIXES: .S .s .c .o .a .d .wav .raw

.wav.s: $(WAVFILES)
	@echo \# converting $<
	@sox $< -r 16384 -c 1 -B -s $*.raw
	@padbin 4 $*.raw
	@bin2s $*.raw > $@
	@rm $*.raw
