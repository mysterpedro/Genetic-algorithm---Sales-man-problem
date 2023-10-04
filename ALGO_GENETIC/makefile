CC = gcc
CFLAGS = -Wall -g

SRC = algoGenetic.c
OBJ = $(SRC:.c=.o)
EXEC = algoGenetic

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(EXEC) $(OBJ)
