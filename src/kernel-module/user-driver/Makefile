EXEC	= dreamyfan
COMMON_SRCS	= ../../common/pbm_utils.c ../../common/config.c
SRCS	= $(COMMON_SRCS) main.c
OBJS 	= $(SRCS:.c=.o)

CC	= gcc
INCS	= -I../module -I../../common -I/usr/include/netpbm
CFLAGS	= -g3 -Wall -Werror -std=c99 $(INCS)
LIBS	+= -lnetpbm

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXEC)
