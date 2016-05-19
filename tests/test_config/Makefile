DEBUG=yes

ifeq ($(DEBUG),yes)
	CFLAGS=-g3 -Wall -Werror -std=c99
	LDFLAGS=
else
	CFLAGS=
	LDFLAGS=
endif
CC=gcc
HEADERFILES = includes/cfan_api.h includes/cfan_data.h
SRCS = test.c cfan_api.c read_config.c
OBJS = $(SRCS:.c=.o)
EXEC = test

all: $(EXEC)

$(EXEC): test.o cfan_api.o /usr/lib/libpbm.a
	$(CC) $(CFLAGS) $^ -o test

test.o: test.c $(HEADERFILES)
	$(CC) $(CFLAGS) -c test.c -o test.o

cfan_api.o: cfan_api.c
	$(CC) $(CFLAGS) -c cfan_api.c

.PHONY: clean

clean:
	$(RM) *.o $(EXEC)
