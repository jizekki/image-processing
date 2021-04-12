ROOT=../bcl

CPPFLAGS = -I$(ROOT)/include -I. 
CFLAGS   = -Wall -Wextra -pedantic -std=c99 
LDFLAGS  = -L$(ROOT)/lib 
LDLIBS   = -lbcl -lm -lfftw3

BIN=copy padding filter-zoom

.PHONY: all
all: $(BIN)

padding: fft.o

.PHONY: clean cleanall
clean: cleanall
	$(RM) *.o *.ppm
cleanall: 
	$(RM) $(BIN)

