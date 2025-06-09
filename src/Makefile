.default: all

all: mazeRunner

clean:
	rm -f mazeRunner *.o

mazeRunner: mazeRunner.o
# g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ -lmcpp Maze.cpp
	g++ -Wall -Werror -std=c++17 -O -g -o mazeRunner ./*.cpp -lmcpp
%.o: %.cpp
	g++ -Wall -Werror -std=c++17 -g -O -c $^