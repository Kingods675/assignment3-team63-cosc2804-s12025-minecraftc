#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "Maze.h"

#include <mcpp/mcpp.h>



Maze::Maze(mcpp::MinecraftConnection &mc) : mc(mc) {};

void Maze::build(std::vector<std::vector<char>> maze)
{
    this->maze = maze;
}

std::vector<std::vector<char>> &Maze::getMaze()
{
    return this->maze;
}

void Maze::print()
{
    std::cout << "**Printing Maze Structure**" << std::endl;
    for (unsigned int z = 0; z < this->maze.size(); z++)
    {
        for (unsigned int x = 0; x < this->maze[0].size(); x++)
        {
            if (this->maze[z][x] == 'x')
            {
                std::cout << 'x';
            }
            else
            {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
    std::cout << "**End Prsize_ting Maze**" << std::endl;
    std::cout << std::endl;
}

void dfs(std::vector<std::vector<char>> &maze, size_t row, size_t col)
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

void floodFill(std::vector<std::vector<char>> &maze, size_t row, size_t col)
{

    // changing . to o
    // if (maze[row][col] == 'o')
    // {
    //     std::cout << "changing to o: row = " << row << "col = " << col << std::endl;
    //     return;
    // }

    // // Call DFS to start filling
    // std::cout << "calling dfs: " << row << " - " << col << std::endl;
    dfs(maze, row, col);
}

bool Maze::validateIsolations()
{
    std::vector<std::vector<char>> copy = this->maze;
    bool filled = false;

    // floodFill(copy, 1, 4);

    for (size_t i = 0; i < copy.size() && !filled; i++)
    {
        for (size_t j = 0; j < copy[i].size() && !filled; j++)
        {
            if (copy[i][j] == '.')
            {
                floodFill(copy, i, j);
                // std::cout << "Flood fill at i-j = " << i << j << std::endl;
                filled = true;
            }
        }
    }

    this->floodedMaze = copy;
    

    // std::cout << "\n>> FLOODED MAZE: " << std::endl;
    // for (vector<char> row : floodedMaze)
    // {
    //     for (char c : row)
    //     {
    //         std::cout << c << " ";
    //     }
    //     std::cout << std::endl;
    // }

    for (std::vector<char> row : copy)
    {
        for (char c : row)
        {
            if (c == '.')
                return false;
        }
        // std::cout << std::endl;
    }

    return true;

    // prsize_tMaze(copy);
}

void Maze::fixIsolations()
{
    // std::cout << "> before fixing isolations..." << std::endl;

    // for (vector<char> row : this->floodedMaze)
    // {
    //     for (char c : row)
    //     {
    //         std::cout << c << " ";
    //     }
    //     std::cout << std::endl;
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
    // std::cout << "> after fixing isolations..." << std::endl;

    // for (vector<char> row : this->maze)
    // {
    //     for (char c : row)
    //     {
    //         std::cout << c << " ";
    //     }
    //     std::cout << std::endl;
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
                    std::this_thread::sleep_for(std::chrono::milliseconds(100) );
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
    mcpp::Coordinate entranceCoor = this->origin + mcpp::Coordinate(entrance.j + 1, 0, entrance.i + 1);

    mcpp::Coordinate outsideEntranceCoor = entranceCoor;
    if (entrance.i == 0) {
        // Top wall entrance - place carpet outside at z-1
        outsideEntranceCoor.z -= 1;
    } else if (entrance.i == (int) maze.size() - 1) {
        // Bottom wall entrance - place carpet outside at z+1
        outsideEntranceCoor.z += 1;
    } else if (entrance.j == 0) {
        // Left wall entrance - place carpet outside at x-1
        outsideEntranceCoor.x -= 1;
    } else if (entrance.j == (int) maze[0].size() - 1) {
        // Right wall entrance - place carpet outside at x+1
        outsideEntranceCoor.x += 1;
    }
    // std::cout << "Drawing entrance: " << this->entrance.i << ":" << this->entrance.j << std::endl;
    mc.setBlock(outsideEntranceCoor, mcpp::Blocks::BLUE_CARPET);
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

void Maze::checkEntrace() {

    for (size_t x = 1; x < maze[0].size() - 1; x++)
    {
        if (maze[0][x] == '.')
        {
            entrance.i = 0;
            entrance.j = x;
            break;
        }
    }

    for (size_t x = 1; x < maze[0].size() - 1; x++)
        {
            if (maze[maze.size() - 1][x] == '.')
            {
                entrance.i = maze.size() - 1;
                entrance.j = x;
                break;
            }
        }

    for (size_t z = 1; z < maze.size() - 1; z++)
        {
            if (maze[z][0] == '.')
            {
                entrance.i = z;
                entrance.j = 0;
                break;
            }
        }

    for (size_t z = 1; z < maze.size() - 1; z++)
        {
            if (maze[z][maze[0].size() - 1] == '.')
            {
                entrance.i = z;
                entrance.j = maze[0].size() - 1;
                break;
            }
        }
}

bool Maze::validateLoops()
{
    std::vector<std::vector<char>> copy = this->maze;
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

    // std::cout << "> reverse maze" << std::endl;

    // for (vector<char> row : copy)
    // {
    //     for (char c : row)
    //     {
    //         std::cout << c << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // Perform flood fill from top-left corner
    floodFill(copy, 0, 0);

    // std::cout << "\n>> FLOODED MAZE: " << std::endl;
    // for (vector<char> row : copy)
    // {
    //     for (char c : row)
    //     {
    //         std::cout << c << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // Check for remaining 'x's (untreated walls)
    for (size_t i = 0; i < copy.size(); i++)
    {
        for (size_t j = 0; j < copy[0].size(); j++)
        {
            if (copy[i][j] == '.')
            {
                return false; // Loop detected
            }
        }
    }

    return true; // No loops found
}

void Maze::fixLoops()
{
    std::vector<std::vector<char>> copy = this->maze;
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

    // std::cout << "> after fixing loops..." << std::endl;

    // for (vector<char> row : this->maze)
    // {
    //     for (char c : row)
    //     {
    //         std::cout << c << " ";
    //     }
    //     std::cout << std::endl;
    // }
}

void Maze::setOrigin(mcpp::Coordinate origin)
{
    this->origin = origin;
}