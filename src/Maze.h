#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <mcpp/mcpp.h>

using namespace std;

class Maze {
    private:
        vector<vector<char>> maze;
        vector<vector<char>> floodedMaze;

        mcpp::Coordinate origin;
        mcpp::MinecraftConnection mc;

        struct BlockChange {
        mcpp::Coordinate pos;
        mcpp::BlockType originalBlock;
        };

        BlockChange* changes; // Dynamic array to store changes
        int changeCount;
        int changeCapacity;

    public:
        void build(vector<vector<char>> maze);
        vector<vector<char>>& getMaze();
        void print();

        bool validateIsolations();
        void fixIsolations();

        bool validateLoops();
        void fixLoops();

        bool hasValidEntrance();
        void fixEntrance();
        
        void draw();
        void deleteMaze();

        ~Maze() {
        if (changes) {
            delete[] changes;
        }
    }
};

