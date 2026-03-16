CC = gcc
CFLAGS = -static -O2 -Wall

TARGET = init
SRC = init.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
