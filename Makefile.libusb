EXE         = pearlfan
COMMON_SRCS = src/common/config.c \
			  src/common/draw.c \
			  src/common/ioutils.c \
			  src/common/optutils.c \
			  src/common/raster.c
SRCS        = $(COMMON_SRCS) src/libusb/usb.c src/libusb/main.c
OBJS        = $(SRCS:.c=.o)

CC      = gcc
INCS    = -Isrc/common -I/usr/include/netpbm
CFLAGS  = -g3 -Wall -Werror -std=c99 $(INCS)
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
