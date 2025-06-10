
#include <iostream>
#include "mcpp/mcpp.h"

#include <list>
#include <chrono>
#include <thread>
#include <vector>

class solveMaze {

    public:
        void setMaze(const std::vector<std::vector<char>>& maze, 
        const mcpp::Coordinate& basePoint);
        void breadthFirstSearch();
        void solveMazeManually(bool state);
        bool checkBoundaries(const mcpp::Coordinate& pos);



    private:
        //tracking relationships between previous and current blocks (For creating path)
        std::list<std::pair<mcpp::Coordinate, mcpp::Coordinate>> previous; 
        //for storing neighbouring blocks to check
        std::queue<mcpp::Coordinate> queue; 
        //to track already visited blocks
        std::list<mcpp::Coordinate> visited;
        bool exitFound;

        mcpp::Coordinate startPos;
        mcpp::Coordinate exitPos;
        //array for storing directions (right, left, forward, back);
        std::vector<mcpp::Coordinate> moveTo;
        mcpp::Coordinate base;
        std::vector<std::vector<char>> mazeInput;
        






};










