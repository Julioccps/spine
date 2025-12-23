CC = gcc
CFLAGS = -Wall -Wextra -Isrc
SRC = src/main.c src/core/decoder.c
OBJ = $(SRC:.c=.o)
TARGET = spine

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
