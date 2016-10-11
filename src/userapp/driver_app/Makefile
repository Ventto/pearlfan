INCLUDES =-I../../driver/includes -I../common
DEBUG = yes

ifeq ($(DEBUG),yes)
	CFLAGS=-g3 -Wall -Werror -std=c99 $(INCLUDES)
else
	CFLAGS=-Wall -Werror -std=c99 $(INCLUDES)
endif

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
