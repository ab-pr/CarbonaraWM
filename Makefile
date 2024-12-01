CC = g++
CFLAGS = -Wall -std=c++11 -O3
TARGET = main

# Source files
SRC = src/main.cpp src/parse_keys.cpp src/key_listen.cpp
HEADERS = src/parse_keys.h src/key_listen.h

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -framework ApplicationServices -framework Carbon

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)