#AUTEUR	: 43256 Touda lachiri Anas

CFLAGS= -O3 -g -Wall -std=c++17 -I.
CC=g++

build : build/rainbowTable 

build/rainbowTable : build/rainbowTableGen.o build/rainbowAttack.o build/main.o
	${CC} ${CFLAGS} build/*.o -o build/rainbowTable

build/main.o : src/main.cpp
	${CC} ${CFLAGS} src/main.cpp -c -o build/main.o

build/rainbowTableGen.o : src/rainbowTableGen.cpp src/rainbowTableGen.hpp build/sha256.o src/reduction.hpp
	${CC} ${CFLAGS} src/rainbowTableGen.cpp -c -o build/rainbowTableGen.o 

build/rainbowAttack.o : src/rainbowAttack.cpp src/rainbowAttack.hpp
	${CC} ${CFLAGS} src/rainbowAttack.cpp -c -o build/rainbowAttack.o

build/sha256.o: src/sha256.cpp src/sha256.h
	${CC} ${CFLAGS} src/sha256.cpp -c -o build/sha256.o

clean:
	rm -f build/* 
