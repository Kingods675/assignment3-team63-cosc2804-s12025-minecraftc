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
        mcpp::MinecraftConnection mc;

    public:
        void build(vector<vector<char>> maze);
        vector<vector<char>>& getMaze();
        void print();

        bool validateIsolations();
        void fixIsolations();

        void draw();
};