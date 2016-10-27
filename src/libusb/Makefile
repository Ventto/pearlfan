EXEC	= dreamyfan
COMMON_SRCS = ../common/pbm_utils.c ../common/config.c
SRCS	= $(COMMON_SRCS) libusb_utils.c main.c
OBJS 	= $(SRCS:.c=.o)

CC		= gcc
INCS	= -I../kernel-module/module -I../common -I/usr/include/netpbm
CFLAGS	= -Wall -Werror -std=c99 $(INCS)
LIBS	+= $(shell pkg-config --libs --cflags libusb-1.0)
LIBS	+= -lnetpbm

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(EXEC) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXEC)
