#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "Maze.h"

#include <mcpp/mcpp.h>

using namespace std;

void Maze::build(vector<vector<char>> maze)
{
    this->maze = maze;
}

vector<vector<char>> &Maze::getMaze()
{
    return this->maze;
}

void Maze::print() {
    cout << "**Printing Maze Structure**" << endl;
    for (unsigned int z = 0; z < this->maze.size(); z++){
        for (unsigned int x = 0; x < this->maze[0].size(); x++){
            if(this->maze[z][x] == 'x'){
                cout << 'x';
            }
            else
            {
                cout << '.';
            }
        }
        cout << endl;
    }
    cout << "**End Prsize_ting Maze**" << endl;
    cout << endl;
}

void dfs(vector<vector<char>> &maze, size_t row, size_t col)
{

    // Base case: check boundary conditions and wrong char
    if (row < 0 || row >= maze.size() ||
        col < 0 || col >= maze[0].size() ||
        maze[row][col] != '.')
    {
        return;
    }

    // Update the char of the current pixel
    maze[row][col] = 'o';

    // Recursively visit all 4 connected neighbors
    dfs(maze, row + 1, col);
    dfs(maze, row - 1, col);
    dfs(maze, row, col + 1);
    dfs(maze, row, col - 1);
}

void floodFill(vector<vector<char>> &maze, size_t row, size_t col)
{

    // changing . to o
    if (maze[row][col] == 'o')
    {
        cout << "changing to o: row = " << row << "col = " << col << endl;
        return;
    }

    // Call DFS to start filling
    cout << "calling dfs: " << row << " - " << col << endl;
    dfs(maze, row, col);
}

bool Maze::validateIsolations()
{
    vector<vector<char>> copy = this->maze;

    // TODO: check if char is a dot before calling floodfill
    floodFill(copy, 1, 4);

    this->floodedMaze = copy;

    cout << "\n>> FLOODED MAZE: " << endl;
    for (vector<char> row : floodedMaze)
    {
        for (char c : row)
        {
            cout << c << " ";
        }
        cout << endl;
    }

    // for (vector<char> row : copy) {
    //     for (char c : row) {
    //         cout << c << " ";
    //     }
    //     cout << endl;
    // }

    for (vector<char> row : copy)
    {
        for (char c : row)
        {
            if (c == '.')
                return false;
        }
        cout << endl;
    }

    return true;

    // prsize_tMaze(copy);
}

void Maze::fixIsolations()
{
    cout << "> before fixing isolations..." << endl;

    for (vector<char> row : this->floodedMaze)
    {
        for (char c : row)
        {
            cout << c << " ";
        }
        cout << endl;
    }

    // fixing...

    for (size_t i = 1; i < this->floodedMaze.size() - 1; i++)
    {
        // bool cont = true;
        for (size_t j = 1; j < this->floodedMaze[i].size() - 1; j++)
        {
            if (this->floodedMaze[i][j] == '.')
            {
                // TODO: check if i and j are in bound
                // ...
                // NOTE: current not correct yet

                // size_t x = -1, y = -1;

                // check if TOP wall is breakable
                if (i >= 2 && this->floodedMaze[i - 2][j] == 'o')
                {
                    // replacing the wall from ACTUAL MAZE from 'x' to '.'
                    this->maze[i - 1][j] = '.';
                    // x = i-1;
                    // y = j;
                    break;
                }

                // check if BOTTOM wall is breakable
                else if (i + 2 < this->floodedMaze.size() - 1 && this->floodedMaze[i + 2][j] == 'o')
                {
                    // replacing the wall from ACTUAL MAZE from 'x' to '.'
                    this->maze[i + 1][j] = '.';
                    // x = i+1;
                    // y = j;
                    break;
                }

                // check if RIGHT wall is breakable
                else if (j + 2 < this->floodedMaze[i].size() - 1 && this->floodedMaze[i][j + 2] == 'o')
                {
                    // replacing the wall from ACTUAL MAZE from 'x' to '.'
                    this->maze[i][j + 1] = '.';
                    // x = i;
                    // y = j+1;
                    break;
                }

                // check if LEFT wall is breakable
                else if (j >= 2 && this->floodedMaze[i][j - 2] == 'o')
                {
                    // replacing the wall from ACTUAL MAZE from 'x' to '.'
                    this->maze[i][j - 1] = '.';
                    // x = i;
                    // x = i-1;
                    break;
                }
            }
        }
    }
    cout << "> after fixing isolations..." << endl;

    for (vector<char> row : this->maze)
    {
        for (char c : row)
        {
            cout << c << " ";
        }
        cout << endl;
    }
}


void Maze::draw() {
    mcpp::Coordinate origin = mc.getPlayerPosition(); // x y z
    mcpp::Coordinate pos = origin;

    // for (size_t i = 0; i < maze.size() + 2; i++) {
    //         for (size_t j = 0; j <= maze[i].size() + 2; j++) {
    //             mc.setBlock(pos, mcpp::Blocks::AIR);
    //             std::this_thread::sleep_for(std::chrono::milliseconds(100) );
    //             pos.x++;
    //         }
    //         pos.z++;
    //         pos.x = origin.x;
    //         cout << endl;
    //     }

    for (size_t y = 0; y < 3; y++)
    {   
        pos.x = origin.x + 1;
        pos.z = origin.z + 1;
        for (size_t i = 0; i < maze.size(); i++)
        {
            for (size_t j = 0; j < maze[i].size(); j++)
            {
                if (maze[i][j] == 'x')
                {
                    mc.setBlock(pos, mcpp::Blocks::STONE);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100) );
                }
                pos.x++;
            }
            pos.z++;
            pos.x = origin.x + 1;
            cout << endl;
        }
        pos.y++;
    }
}