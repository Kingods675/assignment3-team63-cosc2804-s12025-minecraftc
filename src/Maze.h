#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <mcpp/mcpp.h>


struct BlockChange {
    mcpp::Coordinate pos;
    mcpp::BlockType originalBlock;
};

struct Entrance {
    int i;
    int j;
};

class Maze {
    private:
        std::vector<std::vector<char>> maze;
        std::vector<std::vector<char>> floodedMaze;

        mcpp::MinecraftConnection& mc;
        mcpp::Coordinate origin;
        
        BlockChange* changes; // Dynamic array to store changes
        int changeCount;
        int changeCapacity;
        Entrance entrance;

    public:
        // constructor
        Maze(mcpp::MinecraftConnection& conn);

        // methods
        void build(std::vector<std::vector<char>> maze);
        std::vector<std::vector<char>>& getMaze();
        void print();

        bool validateIsolations();
        void fixIsolations();

        bool validateLoops();
        void fixLoops();

        bool hasValidEntrance();
        void fixEntrance();
        void checkEntrace();
        
        void draw(bool mode);
        void deleteMaze();

        void setOrigin(mcpp::Coordinate origin);

        ~Maze() {
        if (changes) {
            delete[] changes;
        }
    }
};

