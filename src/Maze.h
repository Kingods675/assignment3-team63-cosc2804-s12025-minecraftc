#include <iostream>
#include <vector>

#include <mcpp/mcpp.h>

using namespace std;

class Maze {
    private:
        vector<vector<char>> maze;

    public:
        void build(vector<vector<char>> maze);
        vector<vector<char>>& getMaze();
        void print();

        bool validateIsolations();
};