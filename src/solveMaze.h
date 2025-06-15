#include <iostream>
#include "mcpp/mcpp.h"
#include <queue>
#include <list>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib>


class solveMaze {

    public:

        //solve maze functions
        void setMaze(const std::vector<std::vector<char>>& maze, 
        
        const mcpp::Coordinate& basePoint);
        void breadthFirstSearch(bool mazeExist, bool state);
        void solveMazeManually(bool state, bool mazeExist);
        bool checkBoundaries(const mcpp::Coordinate& pos, bool state);
        void buildEscapeRoute(bool mazeExist, bool state);
        



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










