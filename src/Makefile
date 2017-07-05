CPP = g++
EXE = CrazyBuyer
OBJ = main.o
CFLAGS =  -ansi -pedantic -std=c++11 -ggdb -c

EXE: $(OBJ)
	$(CPP) -o $(EXE) $(OBJ)

clean: 
	rm -f $(OBJ) $(EXE)

main.o: main.cpp Common.h json.hpp utils.cpp
	$(CPP) $(CFLAGS) main.cpp

utils.o: utils.cpp Common.h json.hpp
	$(CPP) $(CFLAGS) utils.cpp
