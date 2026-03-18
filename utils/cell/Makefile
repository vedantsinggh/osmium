CC = gcc
CFLAGS = -static -O2 -Wall

TARGET = cell
SRC = cell.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
