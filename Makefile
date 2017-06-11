CC=g++ -std=c++11
CFLAGS= -c -Wall
LDFLAGS=
SOURCES= main.cpp CellularAutomata.cpp NumberGenerator.cpp Evolution.cpp Chromosome.cpp
OBJECTS= $(SOURCES:.cpp=.o)
EXECUTABLE= BIN_EVO_projekt

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
