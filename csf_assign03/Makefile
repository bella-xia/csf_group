CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: main.o
	g++ -o csim main.o

main.o: main.cpp
	g++ -c main.cpp $(CFLAGS)

.PHONY: clean

solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt  *.cpp

clean:
	rm -f *.o csim *~