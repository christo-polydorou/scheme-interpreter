
SRC_DIR = src
INCLUDE_DIR = include

SRCS = $(wildcard $(SRC_DIR)/*.c)
HDRS = $(wildcard $(INCLUDE_DIR)/*.h)

CC = clang
CFLAGS = -g

OBJS = $(SRCS:.c=.o)

.PHONY: interpreter
interpreter: $(OBJS)
	$(CC)  $(CFLAGS) $^  -o $@
	rm -f *.o
	rm -f vgcore.*

clean:
	rm -f *.o
	rm -f interpreter

