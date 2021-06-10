CXX = g++
CPPFLAGS = -g -Wall -std=c++11 

all: scrabble

valgrind: 
	valgrind --tool=memcheck --leak-check=yes ./scrabble config.txt

gdb:
	gdb ./scrabble config.txt

scrabble: scrabble.cpp dictionary.o player.o board.o bag.o console.o move.o
	$(CXX) $(CPPFLAGS) $^ -o $@

move.o: Move.cpp Move.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

dictionary.o: Dictionary.cpp Dictionary.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@

player.o: Player.cpp Player.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@

board.o: Board.cpp Board.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@


bag.o: Bag.cpp Bag.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@ 

console.o: ConsolePrinter.cpp ConsolePrinter.h rang.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -rf scrabble dictionary.o player.o move.o bag.o board.o console.o 