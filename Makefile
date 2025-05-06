CC = g++
CFLAGS = -std=c++11 -Wall -g3
LDFLAGS = -pthread

# Cibles
TARGET_SEQ = labyrinthe_seq
TARGET_PAR = labyrinthe_par

# Fichiers source
SRC_SEQ = main.cpp Labyrinthe_seq.cpp
SRC_PAR = main_par.cpp Labyrinthe_par.cpp Labyrinthe_seq.cpp

# Objets
OBJ_SEQ = $(SRC_SEQ:.cpp=.o)
OBJ_PAR = $(SRC_PAR:.cpp=.o)

all: $(TARGET_SEQ) $(TARGET_PAR)

# Cible séquentielle
$(TARGET_SEQ): $(OBJ_SEQ)
	$(CC) $(CFLAGS) -o $@ $^

# Cible parallèle
$(TARGET_PAR): $(OBJ_PAR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET_SEQ) $(TARGET_PAR)

rebuild: clean all