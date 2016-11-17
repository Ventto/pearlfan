EXE         = pearlfan
CORE_SRCS   = src/core/config.c \
              src/core/draw.c \
              src/core/ioutils.c \
              src/core/optutils.c \
              src/core/raster.c
SRCS        = $(CORE_SRCS) src/libusb/usb.c src/libusb/main.c
OBJS        = $(SRCS:.c=.o)

CC      = gcc
INCS    = -Isrc/core -I/usr/include/netpbm
CFLAGS  = -Wall -Werror -std=c99 $(INCS)
LIBS   += $(shell pkg-config --libs --cflags libusb-1.0)
LIBS   += -lnetpbm

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(EXE) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXE)
