CC=cc
CFLAGS=-g -O2 -lncurses -lcurl -lgit2 -Wall

OBJ=obj
SRC=src
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))

all: mine

mine: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c $< -o $@

clean:
	rm mine $(OBJ)/*
