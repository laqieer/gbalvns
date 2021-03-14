.PHONY: all clean

all:
	make -C core
	make -C asset
	make $(GAME)

clean:
	make -C core clean
	make -C asset clean
	rm -f $(GAME)

GAME = summer.gba

$(GAME): core/core.gba asset/asset.gbfs
	@echo \# linking $@
	@cat $^ > $@

