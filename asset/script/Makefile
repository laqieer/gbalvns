PREFIX  = arm-none-eabi-

CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
CFLAGS = -fextended-identifiers

DEPS = $(wildcard *.h) $(wildcard *.s)
SCRIPTS = $(wildcard *.S)

DATS = $(addsuffix .dat, $(basename $(notdir $(SCRIPTS))))


%.o: %.S $(DEPS)
	@echo \# assembling $<
	@$(CC) -c -o $@ $< $(CFLAGS)

%.dat: %.o
	@echo \# dumping $@
	@$(OBJCOPY) -O binary $< $@

.PHONY: all clean

all: $(DATS)

clean:
	rm -f $(DATS) *.o
