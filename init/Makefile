CC = g++
CFLAGS = -static -O2 -Wall -std=c++17 -pedantic

TARGET = init
SRC = init.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
