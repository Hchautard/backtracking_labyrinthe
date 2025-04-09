CC = g++
CFLAGS = -std=c++11 -Wall -g3
TARGET = labyrinthe_seq
SRC = main.cpp Labyrinthe_seq.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all