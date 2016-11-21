EXE    = pearltest
SRCS   = ../../src/draw/draw.c \
         ../../src/usb.c \
         tests.c \
         main.c
OBJS   = $(SRCS:.c=.o)

CC     = gcc
INCS   = -I../../src -I../../src/draw -I../../src/utils
CFLAGS = -Wall -Werror -std=c99 $(INCS)
LIBS   += $(shell pkg-config --libs --cflags libusb-1.0)

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(EXE) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXE)
