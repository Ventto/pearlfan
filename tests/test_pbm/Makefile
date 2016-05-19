DEBUG=yes

ifeq ($(DEBUG),yes)
	CFLAGS=-g3 -Wall -Werror -std=c99
else
	CFLAGS=
endif
CC = gcc
HEADERFILES =
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
LIBS = /usr/lib/libpbm.a
EXEC = test_pbm

all: $(EXEC)

$(EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $(EXEC)

%.o: %.c $(HEADERFILES)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) *.o $(EXEC)
