# Variables

CXX = g++
CXXFLAGS = -Wall -g -I /usr/include/eigen3 -std=c++11 -fopenmp
CXXFLAGS_STATIC = -Wall -g -I /usr/include/eigen3 -std=c++11 -static -fopenmp

# Targets
all: main

main: main.o Rep3D.o
	$(CXX) $(CXXFLAGS) -o main main.o Rep3D.o

static: main.o Rep3D.o
	$(CXX) $(CXXFLAGS_STATIC) -o main main.o Rep3D.o

main.o: main.cpp Rep3D.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Rep3D.o: Rep3D.cpp Rep3D.h
	$(CXX) $(CXXFLAGS) -c Rep3D.cpp

clean:	
	rm -f *.o
	rm -f *.txt
	rm -f *.csv
	rm -f *.wrl