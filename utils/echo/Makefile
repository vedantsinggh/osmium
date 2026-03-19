CC = gcc
CFLAGS = -static -O2 -Wall

TARGET = echo
SRC = echo.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
