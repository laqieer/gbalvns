.PHONY: all clean

all:
	make -C script
	make -C image
	make -C sound
	make $(GBFS)

clean:
	make -C script clean
	make -C image clean
	make -C sound clean
	rm -f $(GBFS)

GBFS = asset.gbfs

$(GBFS): script image sound
	@echo \# packing $@
	@rm -f $@
	@gbfs $@ script/*.dat image/*.img sound/*.8ad

