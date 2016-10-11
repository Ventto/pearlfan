INCLUDES =-I../../driver/includes -I../common -I/usr/include/netpbm
CFLAGS		=-Wall -Werror -std=c99 $(INCLUDES)

CC = gcc
HEADERFILES =
SRCS = $(wildcard *.c) $(wildcard ../common/*.c)
OBJS = $(SRCS:.c=.o)
LIBS += $(shell pkg-config --libs --cflags libusb-1.0)
LIBS += -lnetpbm
EXEC = app

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(EXEC) $(LIBS)

%.o: %.c $(HEADERFILES)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) *.o $(EXEC)
