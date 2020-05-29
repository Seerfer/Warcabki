CX1FLAGS= -lsfml-graphics -lsfml-window -lsfml-system --std=c++14
CX2FLAGS= --std=c++14
 
__start__: ./Draughts clean
	./Draughts
 
./Draughts: main.o Draughts.o AI.o Board.o
	g++ -o ./Draughts main.o Draughts.o AI.o Board.o ${CX1FLAGS}

main.o: main.cpp Draughts.hpp AI.hpp Board.hpp
	g++ -c -o main.o main.cpp ${CX1FLAGS}

Draughts.o: Draughts.cpp Draughts.hpp AI.hpp Board.hpp
	g++ -c -o Draughts.o Draughts.cpp ${CX1FLAGS}

AI.o: AI.cpp AI.hpp Board.hpp
	g++ -c -o AI.o AI.cpp ${CX2FLAGS}

Board.o: Board.cpp Board.hpp
	g++ -c -o Board.o Board.cpp ${CX2FLAGS}
 
clean: 
	rm -f *.o
