EXE    = pearlfan
SRCS   = src/config/config.c \
			src/config/raster.c \
			src/draw/draw.c \
			src/utils/ioutils.c \
			src/utils/optutils.c \
			src/usb.c \
			src/main.c
OBJS   = $(SRCS:.c=.o)

CC     = gcc
INCS   = -Isrc/config -Isrc/draw -Isrc/utils -Isrc \
		 -I/usr/include/netpbm
CFLAGS = -Wall -Werror -std=c99 $(INCS)
LIBS   += $(shell pkg-config --libs --cflags libusb-1.0)
LIBS   +=  -lnetpbm

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(EXE) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXE)
