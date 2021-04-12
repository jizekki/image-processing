
ROOT = ../bcl

CPPFLAGS = -I$(ROOT)/include 
CFLAGS   = -Wall -Wextra -Werror -std=c99
LDFLAGS  = -L$(ROOT)/lib
LDLIBS   = -lbcl -lm

BIN=colorization

all: $(BIN)

clean:
	$(RM) $(BIN) *.ppm

test:
	./colorization ../data/insect-c.ppm ../data/insect-g.ppm a.ppm && pvisu a.ppm

.PHONY: all clean

