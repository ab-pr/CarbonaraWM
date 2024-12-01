CC = g++
CFLAGS = -Wall
TARGET = main

# Source files
SRC = main.cpp parse_keys.cpp
HEADERS = parse_keys.h

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)