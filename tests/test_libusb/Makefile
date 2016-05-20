DEBUG=yes
DEBUG = yes

ifeq ($(DEBUG),yes)
	CFLAGS=-g3 -Wall -Werror -std=c99 $(INCLUDES)
else
	CFLAGS=-Wall -Werror -std=c99 $(INCLUDES)
endif

CC = gcc
HEADERFILES =
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
LIBS += $(shell pkg-config --libs --cflags libusb-1.0)
EXEC = test

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(EXEC) $(LIBS)

%.o: %.c $(HEADERFILES)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) *.o $(EXEC)
