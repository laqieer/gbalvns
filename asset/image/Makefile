IMAGES = $(wildcard *.png)

IMGS = $(addsuffix .img, $(basename $(notdir $(IMAGES))))

%.img: %.png
	@echo \# converting $<
	@$(DEVKITPRO)/tools/bin/grit $< -gb -gB16 -ftb -gu16 -gz! -fh!
	@mv $@.bin $@
	@../../tool/imgfix.py $<

.PHONY: all clean

all: $(IMGS)

clean:
	rm -f $(IMGS)
