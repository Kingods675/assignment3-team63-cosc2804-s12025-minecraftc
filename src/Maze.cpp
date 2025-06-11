#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "Maze.h"

#include <mcpp/mcpp.h>

using namespace std;

Maze::Maze(mcpp::MinecraftConnection &mc) : mc(mc) {};

void Maze::build(vector<vector<char>> maze)
{
    this->maze = maze;
}

vector<vector<char>> &Maze::getMaze()
{
    return this->maze;
}

void Maze::print()
{
    cout << "**Printing Maze Structure**" << endl;
    for (unsigned int z = 0; z < this->maze.size(); z++)
    {
        for (unsigned int x = 0; x < this->maze[0].size(); x++)
        {
            if (this->maze[z][x] == 'x')
            {
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
    if (row >= maze.size() || col >= maze[0].size() ||
        maze[row][col] != '.')
    {
        return;
    }

    // Update the char of the current pixel
    maze[row][col] = 'o';

    // Recursively visit all 4 connected neighbors
    if (row > 0)
        dfs(maze, row - 1, col);
    if (row < maze.size() - 1)
        dfs(maze, row + 1, col);
    if (col > 0)
        dfs(maze, row, col - 1);
    if (col < maze[0].size() - 1)
        dfs(maze, row, col + 1);
}

void floodFill(vector<vector<char>> &maze, size_t row, size_t col)
{

    // changing . to o
    // if (maze[row][col] == 'o')
    // {
    //     cout << "changing to o: row = " << row << "col = " << col << endl;
    //     return;
    // }

    // // Call DFS to start filling
    // cout << "calling dfs: " << row << " - " << col << endl;
    dfs(maze, row, col);
}

bool Maze::validateIsolations()
{
    vector<vector<char>> copy = this->maze;

    // floodFill(copy, 1, 4);

    for (size_t i = 0; i < copy.size(); i++)
    {
        for (size_t j = 0; j < copy[i].size(); j++)
        {
            if (copy[i][j] == '.')
            {
                floodFill(copy, i, j);
                cout << "Flood fill at i-j = " << i << j << endl;
                break;
            }
        }
    }

    this->floodedMaze = copy;

    // cout << "\n>> FLOODED MAZE: " << endl;
    // for (vector<char> row : floodedMaze)
    // {
    //     for (char c : row)
    //     {
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
    // cout << "> before fixing isolations..." << endl;

    // for (vector<char> row : this->floodedMaze)
    // {
    //     for (char c : row)
    //     {
    //         cout << c << " ";
    //     }
    //     cout << endl;
    // }

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
    // cout << "> after fixing isolations..." << endl;

    // for (vector<char> row : this->maze)
    // {
    //     for (char c : row)
    //     {
    //         cout << c << " ";
    //     }
    //     cout << endl;
    // }
}

void Maze::draw(bool mode)
{
    // tesing mode
    if (mode == 1)
    {
        mc.setPlayerPosition(mcpp::Coordinate(4848, 71, 4369));
    }

    this->origin = mc.getPlayerPosition(); // x y z
    mcpp::Coordinate pos = origin;

    changeCapacity = (maze.size() + 2) * (maze[0].size() + 2) * 3; // Worst case
    changes = new BlockChange[changeCapacity];
    changeCount = 0;

    // clean up terrain
    for (size_t i = 0; i < maze.size() + 2; i++)
    {
        for (size_t j = 0; j < maze[0].size() + 2; j++)
        {
            pos.y = origin.y - 1;
            if (changeCount < changeCapacity)
            {
                changes[changeCount++] = {pos, mc.getBlock(pos)};
            }
            mc.setBlock(pos, mcpp::Blocks::GRASS);

            pos.y = origin.y;
            if (changeCount < changeCapacity)
            {
                changes[changeCount++] = {pos, mc.getBlock(pos)};
            }
            mc.setBlock(pos, mcpp::Blocks::AIR);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            pos.x++;
        }
        pos.z++;
        pos.x = origin.x;
    }

    // build maze
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
                    mc.setBlock(pos, mcpp::Blocks::ACACIA_WOOD_PLANK);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                pos.x++;
            }
            pos.z++;
            pos.x = origin.x + 1;
        }
        pos.y++;
    }

    // draw entrance carpet
    cout << "Drawing entrance: " << this->entrance.i << ":" << this->entrance.j << endl;
    mcpp::Coordinate entranceCoor = this->origin + mcpp::Coordinate(entrance.j + 1, 0, entrance.i); // TODO: check direction
    mc.setBlock(entranceCoor, mcpp::Blocks::BLUE_CARPET);
}

void Maze::deleteMaze()
{
    mcpp::Coordinate pos = origin;
    for (size_t y = 0; y < 3; y++)
    {
        pos.x = origin.x + 1;
        pos.z = origin.z + 1;
        for (size_t i = 0; i < maze.size(); i++)
        {
            for (size_t j = 0; j < maze[i].size(); j++)
            {
                mc.setBlock(pos, mcpp::Blocks::AIR);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                pos.x++;
            }
            pos.z++;
            pos.x = origin.x + 1;
        }
        pos.y++;
    }

    for (int i = changeCount - 1; i >= 0; i--)
    {
        mc.setBlock(changes[i].pos, changes[i].originalBlock);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Clean up allocated memory
    if (changes)
    {
        delete[] changes;
        changes = nullptr;
    }
    changeCount = 0;
    changeCapacity = 0;
}

bool Maze::hasValidEntrance()
{
    int entranceCount = 0;

    for (size_t x = 0; x < maze[0].size(); x++)
    {
        // Top wall
        if (maze[0][x] == '.')
        {
            entranceCount++;
        }
        // Bottom wall
        if (maze[maze.size() - 1][x] == '.')
        {
            entranceCount++;
        }
    }

    for (size_t z = 1; z < maze.size() - 1; z++)
    {
        // Left wall
        if (maze[z][0] == '.')
        {
            entranceCount++;
        }
        // Right wall
        if (maze[z][maze[0].size() - 1] == '.')
        {
            entranceCount++;
        }
    }

    return entranceCount == 1;
}

void Maze::fixEntrance()
{
    // close all existing openings
    for (size_t x = 0; x < maze[0].size(); x++)
    {
        // Top wall
        if (maze[0][x] == '.')
        {
            maze[0][x] = 'x';
        }
        // Bottom wall
        if (maze[maze.size() - 1][x] == '.')
        {
            maze[maze.size() - 1][x] = 'x';
        }
    }

    for (size_t z = 1; z < maze.size() - 1; z++)
    {
        // Left wall
        if (maze[z][0] == '.')
        {
            maze[z][0] = 'x';
        }
        // Right wall
        if (maze[z][maze[0].size() - 1] == '.')
        {
            maze[z][maze[0].size() - 1] = 'x';
        }
    }

    bool entranceCreated = false;

    // 100% open on top
    for (size_t x = 1; x < maze[0].size() - 1; x++)
    {
        if (maze[1][x] == '.')
        {
            maze[0][x] = '.';
            entranceCreated = true;
            entrance.i = 0;
            entrance.j = x;
            break;
        }
    }

    // If not found, try bottom wall
    if (!entranceCreated)
    {
        for (size_t x = 1; x < maze[0].size() - 1; x++)
        {
            if (maze[maze.size() - 2][x] == '.')
            {
                maze[maze.size() - 1][x] = '.';
                entranceCreated = true;
                entrance.i = maze.size() - 1;
                entrance.j = x;
                break;
            }
        }
    }

    // If not found, try left wall
    if (!entranceCreated)
    {
        for (size_t z = 1; z < maze.size() - 1; z++)
        {
            if (maze[z][1] == '.')
            {
                maze[z][0] = '.';
                entranceCreated = true;
                entrance.i = z;
                entrance.j = 0;
                break;
            }
        }
    }

    // If not found, try right wall
    if (!entranceCreated)
    {
        for (size_t z = 1; z < maze.size() - 1; z++)
        {
            if (maze[z][maze[0].size() - 2] == '.')
            {
                maze[z][maze[0].size() - 1] = '.';
                entranceCreated = true;
                entrance.i = z;
                entrance.j = maze[0].size() - 1;
                break;
            }
        }
    }

    // If still not found (unlikely), create one arbitrarily
    if (!entranceCreated)
    {
        maze[0][1] = '.';
    }
}

bool Maze::validateLoops()
{
    vector<vector<char>> copy = this->maze;
    // Flood fill from top-left corner (treat walls as passages)
    for (size_t i = 0; i < copy.size(); i++)
    {
        for (size_t j = 0; j < copy[0].size(); j++)
        {
            if (copy[i][j] == 'x')
            {
                copy[i][j] = '.'; // Treat walls as passages
            }
            else if (copy[i][j] == '.')
            {
                copy[i][j] = 'x';
            }
            else
            {
                copy[i][j] = 'x'; // Treat passages as walls
            }
        }
    }

    // cout << "> reverse maze" << endl;

    // for (vector<char> row : copy)
    // {
    //     for (char c : row)
    //     {
    //         cout << c << " ";
    //     }
    //     cout << endl;
    // }

    // Perform flood fill from top-left corner
    floodFill(copy, 0, 0);

    // cout << "\n>> FLOODED MAZE: " << endl;
    // for (vector<char> row : copy)
    // {
    //     for (char c : row)
    //     {
    //         cout << c << " ";
    //     }
    //     cout << endl;
    // }

    // Check for remaining 'x's (untreated walls)
    for (size_t i = 0; i < copy.size(); i++)
    {
        for (size_t j = 0; j < copy[0].size(); j++)
        {
            if (copy[i][j] == 'x')
            {
                return false; // Loop detected
            }
        }
    }

    return true; // No loops found
}

void Maze::fixLoops()
{
    vector<vector<char>> copy = this->maze;
    // Flood fill from top-left corner (treat walls as passages)
    for (size_t i = 0; i < copy.size(); i++)
    {
        for (size_t j = 0; j < copy[0].size(); j++)
        {
            if (copy[i][j] == 'x')
            {
                copy[i][j] = '.'; // Treat walls as passages
            }
            else if (copy[i][j] == '.')
            {
                copy[i][j] = 'x';
            }
            else
            {
                copy[i][j] = 'x'; // Treat passages as walls
            }
        }
    }

    floodFill(copy, 0, 0);

    for (size_t i = 1; i < copy.size() - 1; i++)
    {
        for (size_t j = 1; j < copy[0].size() - 1; j++)
        {
            if (copy[i][j] == '.')
            {
                if (i >= 2 && copy[i - 2][j] == 'o')
                {
                    this->maze[i - 1][j] = 'x';
                    break;
                }

                else if (i + 2 < copy.size() - 1 && copy[i + 2][j] == 'o')
                {
                    this->maze[i + 1][j] = 'x';
                    break;
                }

                else if (j + 2 < copy[i].size() - 1 && copy[i][j + 2] == 'o')
                {
                    this->maze[i][j + 1] = 'x';
                    break;
                }

                else if (j >= 2 && copy[i][j - 2] == 'o')
                {
                    this->maze[i][j - 1] = 'x';
                    break;
                }
            }
        }
    }

    // cout << "> after fixing loops..." << endl;

    // for (vector<char> row : this->maze)
    // {
    //     for (char c : row)
    //     {
    //         cout << c << " ";
    //     }
    //     cout << endl;
    // }
}

void Maze::setOrigin(mcpp::Coordinate origin)
{
    this->origin = origin;
}