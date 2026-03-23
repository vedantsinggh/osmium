CC = g++
CFLAGS = -static -std=c++17 -pedantic -O2 -Wall

TARGET = echo
SRC = echo.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
