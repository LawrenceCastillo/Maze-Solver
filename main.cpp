// Sample usage of MazeSolver class

#include <iostream>
#include "MazeSolver.h"

int main() 
{
    MazeSolver solver("input.txt");

    if(solver.mazeIsReady())
    {
        solver.solveMaze();
        solver.printSolution();
    }
   
return 0;
}