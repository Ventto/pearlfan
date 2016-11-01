EXE     = pfan
SRCS    = ../../common/raster.c ../../common/config.c send.c main.c
OBJS    = $(SRCS:.c=.o)

CC      = gcc
INCS    = -I../../common -I/usr/include/netpbm
CFLAGS  = -g3 -Wall -Werror -std=c99 $(INCS)
LIBS   += -lnetpbm

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXE)
