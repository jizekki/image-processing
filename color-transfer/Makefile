ROOT = ../bcl

CPPFLAGS = -I$(ROOT)/include 
CFLAGS   = -Wall -Wextra -Werror -std=c99 -ggdb -fsanitize=address -fno-omit-frame-pointer
LDFLAGS  = -L$(ROOT)/lib
LDLIBS   = -lbcl -lm

BIN=color-transfer

all: $(BIN)

clean:
	$(RM) $(BIN) *.ppm

.PHONY: all clean

