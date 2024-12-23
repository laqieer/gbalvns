WAVS = $(wildcard *.wav)

8ADS = $(addsuffix .8ad, $(basename $(notdir $(WAVS))))

%.8ad: %.wav
	@echo \# converting $<
	@../../tool/wav28ad/wav28ad/wav28ad $< $@
	@rm -f decomp.wav

.PHONY: all clean

all: $(8ADS)

clean:
	rm -f $(8ADS) decomp.wav
