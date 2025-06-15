#include "solveMaze.h"

//store maze dimensions from terminal
void solveMaze:: setMaze(const std::vector<std::vector<char>>& maze, 
    const mcpp::Coordinate& basePoint){

    //store maze
    mazeInput = maze;
    //store basepoint
    base = basePoint;
}


//check if player is within maze
bool solveMaze::checkBoundaries (const mcpp::Coordinate& pos, bool state){

    //store length by getting maze length
    const int wid = mazeInput.size();

    //temporarily make width 0
    int len = 0;

    //if maze length exists, get width
    if (wid > 0){
        len = mazeInput[0].size();
    }

    //check if in boundaries
    bool inBoundaries = false;

    //testmode
    if (state == 1){
    //check if both length and width are greater than 0
        if (len > 0 && wid > 0){
            //if within maze dimensions
            if (pos.x > 4849 && pos.x < 4849 + len
            && pos.z > 4370 && pos.z < 4370 + wid && pos.y < 72 && pos.y >= 71){
                inBoundaries = true;
            }

            //if not,
            else{
                inBoundaries = false;
            }
        }

        else{

            //test output.
        std::cout << "No maze to solve" << std::endl;
        }
    }

    else{

        if (!mazeInput.empty() || !mazeInput[0].empty()){
        //if within maze dimensions
            if (pos.x > base.x && pos.x < base.x + len
            && pos.z > base.z && pos.z < base.z + wid && pos.y < base.y + 2 && pos.y >= base.y){
                inBoundaries = true;
            }

            else{
                inBoundaries = false;
            }
        }
    }

    return inBoundaries;

}

//solve manually functionality
void solveMaze:: solveMazeManually(bool state, bool mazeExist){

    mcpp::MinecraftConnection mc;

    //store length and width
    const int wid = mazeInput.size();
    int len = 0;

    if (wid > 0){
        len = mazeInput[0].size();
    }

    //if in test mode:
    if (state == 1){
        //if built
        if (mazeExist == true){
        //if maze isnt empty
            if (!mazeInput.empty() || !mazeInput[0].empty()){
        
            //initialise variable teleportPos
            mcpp::Coordinate teleportPos = mcpp::Coordinate(4849, 71,4370) + mcpp::Coordinate(len - 2, 0, wid - 2);

            //teleport player to bottom right
            mc.setPlayerPosition(teleportPos);
            std::cout << "Teleporting to: (" << teleportPos.x << ", " << teleportPos.y << ", " << teleportPos.z << ")" << std::endl;
            }

            else{
                std::cout << "No maze to solve" << std::endl;
            }
        }
            //if not built
            else{
                std::cout << "Build maze first" << std::endl;
            }
    }

    //for normal mode
    else{
        //bool for if we can teleport to space
        bool canTeleport = false;

        //check if there is a maze
        if (!mazeInput.empty() || !mazeInput[0].empty()){
            //if built
            if (mazeExist == true){
                mcpp::Coordinate teleportPos;
                //loop until we can find an empty cell to teleport to in maze
                while (!canTeleport){
            
                    //randomx to add to x coordinate
                    int randx = std::rand() % (wid - 2) + 1;
                    //randomz to add to z coordinate
                    int randz = std::rand() % (len - 2) + 1;

                    //target position equal to basepoint + offset

                    teleportPos = base + mcpp::Coordinate(randx, 0, randz);

                    //store blocktype
                    mcpp::BlockType block = mc.getBlock(teleportPos);

                    //not equal to wood (wall), then able to teleport to
                    if (block != mcpp::Blocks::ACACIA_WOOD_PLANK){
                        //break the loop
                        canTeleport = true;
                    }
                }
                //teleport player to coordinate
                mc.setPlayerPosition(teleportPos);
                //print coordinate.
                std::cout << "Teleporting to: (" << teleportPos.x << ", " << teleportPos.y << ", " << teleportPos.z << ")" << std::endl;
            }
            //if not built
            else{
                std::cout << "Build maze first" << std::endl;
            }
        }

        else{
            std::cout << "No maze to solve" << std::endl;

        }
    }
    

}



void solveMaze:: breadthFirstSearch(bool mazeExist, bool state){
    mcpp::MinecraftConnection mc;
    startPos = mc.getPlayerPosition();
    exitPos = mcpp::Coordinate(0,0,0);

    //clear all containers so that users can use bfs search multiple times
    while (!queue.empty()){
        queue.pop();
    } 

    visited.clear();
    previous.clear();
    exitFound = false;

    //initialise queue with starting position
    queue.push(startPos);
    //initialise visitied with starting position
    visited.push_back(startPos);
    //array for storing directions (right, left, forward, back);
    moveTo = {{1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1}};


    if (mazeExist == true){
        if (checkBoundaries(startPos, state)){
        //begin loop
            while (!exitFound && !queue.empty()){ 

                //current added to front of queue
                mcpp::Coordinate curr = queue.front();
                //remove last position
                queue.pop();

                //run for loop that checks if each position one space out is visited
                for (int i = 0; i < 4; ++i){
                    mcpp::Coordinate adjacent {curr.x + moveTo[i].x, curr.y, curr.z + moveTo[i].z};
                    bool blockVisited = false;
                    for (mcpp::Coordinate& v : visited){
                        //if the current block is equal to block being compared, mark as visited
                        if (v.x == adjacent.x && v.y == adjacent.y && v.z == adjacent.z){
                            blockVisited = true;
                        }
                    }

                    //check if block is a wall (wood plank) or not
                    if (!exitFound && !blockVisited){
                        mcpp::BlockType block = mc.getBlock(adjacent);
                        //exit condition (when blue carpet is found)
                        if (block == mcpp::Blocks::BLUE_CARPET){
                            exitFound = true;
                            exitPos = adjacent;
                            previous.push_back({adjacent, curr});
                        }
                        //if not equal to wood plank, then it is walkable
                        else if (block != mcpp::Blocks::ACACIA_WOOD_PLANK){
                            //add to queue
                            queue.push(adjacent);
                            //add to visited list
                            visited.push_back(adjacent);
                            //add to previous list
                            previous.push_back({adjacent, curr});
                            }
                    }
                }
            }

        }
 
    }

}


void solveMaze::buildEscapeRoute(bool mazeExist, bool state){
    mcpp::MinecraftConnection mc;
    
    mcpp::Coordinate currentPos = mc.getPlayerPosition();


    if (mazeInput.empty() || mazeInput[0].empty()){
        std::cout << "No maze to solve" << std::endl;
    }

    else if (mazeExist){
        if (checkBoundaries(currentPos, state)){
            if (exitFound){
                std::vector<mcpp::Coordinate> solvedRoute;
                //start from exit position
                mcpp::Coordinate curr = exitPos;
                bool reachedStartPos = false;
                //traceback path from exit position to beginning
                while(!reachedStartPos){
                    //current position added to end of vector
                    solvedRoute.push_back(curr);
                    //check if we've reached the beginning by comparing coordinates
                    if (curr.x == startPos.x && curr.y == startPos.y && curr.z == startPos.z){
                        reachedStartPos = true;
                    }


                    else{
                        //initalise parent block
                        mcpp::Coordinate previousBlock {-1, -1, -1};
                        for (auto& p : previous){
                            // compare current position and child node
                            if (p.first.x == curr.x && p.first.y == curr.y && p.first.z == curr.z){
                                previousBlock = p.second;
                            }
                        }

                        //set the current position as the previous node.
                        curr = previousBlock;
                    }
                }
                //once path is stored reverse it to get solved route from beginning to end.
                //print coords and highlight route in minecraft


                int step = 0;
                for (int i = solvedRoute.size() - 1; i > 0; --i){
                    mcpp::Coordinate coord = solvedRoute[i];
                    mc.setBlock(mcpp::Coordinate(coord.x, coord.y, coord.z), mcpp::Blocks::LIME_CARPET);
                    std::cout << "step [" << step << "]: (" << coord.x << ", " << coord.y << ", " << coord.z << ")" << std::endl;
                    step++;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    mc.setBlock(mcpp::Coordinate(coord.x, coord.y, coord.z), mcpp::Blocks::AIR);
                
                }
            }

            else{
                std::cout << "Sorry, no path, you are trapped!" << std::endl;

            }
        }
            

        else{
            std::cout << "Get inside maze boundaires" << std::endl;
        }

    }

    else{
        std::cout << "Build maze first" << std::endl;
    }
}


