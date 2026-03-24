CC = g++
CFLAGS = -static -std=c++17 -pedantic -O2 -Wall

TARGET = clear
SRC = clear.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
