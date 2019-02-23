/* 
Title:    MazeSolver.cpp
Author:   Lawrence Gabriel Castillo
Author:   Header created by Prof. Tiziana Ligorio for Hunter College
Date:     2018-Nov-4
Desc:     Attempts to solve user input (text) mazes using south and east movement.
Purpose:  To demonstrate a stack-based solution to traverse and backtrack a path.
Build:    Visual Studio Code 1.31.1; compile: g++ -std=c++11 main.cpp MazeSolver.cpp
*/

#include "MazeSolver.h"
#include <string>
#include <fstream>
#include <iostream>
#include <array>
#include <stack>

//constructor
//pre: input file is in correct format with first two values being integers
//      followed by valid maze characters in {'_', '*','$'}
//post: if inuput file cannot be read outputs "Cannot read from input_file_name"
//      otherwise sets maze_rows_ and maze_columns from first two values in input file
//      and allocates two 2-dimesional array of size maze_rows_ and maze_columns
//      both maze_ and solution_ are initialized with characters read from input
MazeSolver::MazeSolver(std::string input_file)
{
    std::ifstream maze_in;           
    maze_in.open(input_file);  // open file stream

    if (maze_in.fail())  // stop program
        { std::cerr << "Cannot read from " << input_file << std::endl;
        exit(1); }

    else  // read in first two integers; maze_rows_ and maze_columns_
        { int maze_symbol_;

        maze_in >> maze_symbol_;
        maze_rows_ = maze_symbol_;

        maze_in >> maze_symbol_;
        maze_columns_ = maze_symbol_; }

    initializeMaze(maze_rows_, maze_columns_);  // create dynamic array; maze_
    fillMaze(maze_in);  // populate maze_ with file input; after read in of two int values
    maze_in.close();    // close file stream
    
    initializeSolution();   // create dynamic array; solution_
    copyMazetoSolution();   // populate solution_ from maze_
}

// destructor
//post: deletes maze_ and solution_
MazeSolver::~MazeSolver()
{
    for (int i = 0; i < maze_rows_; i++) // iterate through array
        { delete [] maze_[i];
        delete [] solution_[i]; }

    delete [] maze_;
    delete [] solution_;
}

//return: true if maze has been initialized, false otherwise
bool MazeSolver::mazeIsReady()
{
    return maze_ready;  // determined by copyMazetoSolution()
}

//pre: maze_ has been initialized with valid character values in {'_', '*','$'}
//post: solution_ has been marked with '>' signs along the path to the exit
//      prints "Found the exit!!!" if exit is found
//      or "This maze has no solution." if no exit could be found
//return: true if exit is found, false otherwise
bool MazeSolver::solveMaze()
{
    bool maze_solved_ = false;
    Position current_position_;
    current_position_.row = 0;
    current_position_.column = 0;

    backtrack_stack_.push(current_position_);  // initialize stack at 0, 0

    while (!backtrack_stack_.empty() && maze_solved_ == false)

        // return true if char at current position is '$'
        { if (maze_[current_position_.row][current_position_.column] == '$')
            { std::cout << "Found the exit!!!" << std::endl;
            maze_solved_ = true; }

        // move to new position if solution not found
        else if (isExtensible(current_position_, SOUTH) || isExtensible(current_position_, EAST))
            { extendPath(current_position_);  // push new position(s) onto the stack
            solution_[current_position_.row][current_position_.column] = '>';
            current_position_ = backtrack_stack_.top(); } // set current_position_ to new position

        // mark the location and pop the stack
        else
            { maze_[current_position_.row][current_position_.column] = 'X';
            solution_[current_position_.row][current_position_.column] = '@';
            backtrack_stack_.pop();

            if (!backtrack_stack_.empty())
                { current_position_ = backtrack_stack_.top(); }  // take new position

            else
                { std::cout << "This maze has no solution." << std::endl;
                maze_solved_ = false; } } }  // no solution is found when stack is empty 

    return maze_solved_;
}

//post: prints the solution to the standard output stream
//      with single space character between each maze character
void MazeSolver::printSolution()
{
    std::cout << "The solution to this maze is:" << std::endl;

    for (int i = 0; i < maze_rows_; i++)
        { for (int j = 0; j < maze_columns_; j++) 
            { std::cout << solution_[i][j] << " "; }
        std::cout << std::endl; }
}

//PRIVATE MEMBER FUNCTIONS (helper functions)

//pre: rows and columns are positive integers
//post: allocates maze_ with rows and columns
//called by constructor
void MazeSolver::initializeMaze(int rows, int columns)
{
    maze_ = new char*[rows];

    for (int i = 0; i < rows; i++)
        { maze_[i] = new char[columns]; }
}

//pre: maze_ has been allocated with the correct number of rows and columns read from input file
//post: fills in maze_ with characters read from input file
//called by constructor
void MazeSolver::fillMaze(std::ifstream& input_stream)
{
    char maze_symbol_;

    for (int i = 0; i < maze_rows_; i++)
        { for (int j = 0; j < maze_columns_; j++) 
            { input_stream >> maze_symbol_;
            maze_[i][j] = maze_symbol_; } }  // fill maze_ with current character
}

//pre: maze_ has been initialized with valid character values in {'_', '*','$'}
//     start position is always [0][0]
//post: initializes solution_ with a copy of maze_
//      initializes backtrack_stack_  with all viable paths from position [0][0]
//      and mark current position as visited ( > ) on solution_
//called by constructor
void MazeSolver::initializeSolution()
{
    solution_ = new char*[maze_rows_];

    for (int i = 0; i < maze_rows_; i++) 
        { solution_[i] = new char[maze_columns_]; }
}

//pre: maze_ has been properly initialized
//post: allocates solution_ to the correct number of rows and columns
//      and copies the contents of maze_ into solution_
//called by initializeSolution()
void  MazeSolver::copyMazetoSolution()
{
    for (int i = 0; i < maze_rows_; i++)
        { for (int j = 0; j < maze_columns_; j++) 
            { solution_[i][j] = maze_[i][j]; } }  // copy maze_ to solution_

    maze_ready = true;
}

//pre: current_position is a valid position on the maze_
//post: adds all positions extensible from current_position to backtrack_stack_
//return: true if path was extended, false otherwise
//called by solveMaze()
bool MazeSolver::extendPath(Position current_position)
{
    bool extend_path_ = false;

    if (isExtensible(current_position, SOUTH))  // push position to stack if adjacent SOUTH position available
        { backtrack_stack_.push(getNewPosition(current_position, SOUTH));
        extend_path_ = true; }

    if (isExtensible(current_position, EAST))  // push position to stack if adjacent EAST position available
        { backtrack_stack_.push(getNewPosition(current_position, EAST));
        extend_path_ = true; }

    return extend_path_;
}

//pre: old_position is a Position initialized with row and column to valid positions in maze_ and it is extensible in direction dir
//return: a new Position on the maze moving in direction dir from old_position
//called by extendPath()
Position MazeSolver::getNewPosition(Position old_position, direction dir)
{
    Position new_position_;

    if (dir == SOUTH) { new_position_.row = old_position.row + 1; new_position_.column = old_position.column;}  // store new position; SOUTH
    if (dir == EAST) { new_position_.row = old_position.row; new_position_.column = old_position.column + 1; }  // store new position; EAST

    return new_position_;
}

//checks if the path can be extended in maze_ from position current_position in direction dir
//return: true if path can be extended given current_position and dir, false otherwise
//called by extendPath
bool MazeSolver::isExtensible(Position current_position, direction dir)
{
    bool isExtensible = false;

    // SOUTH position is extensible if '_' or '$' characters
    if ((dir == SOUTH) && (current_position.row < maze_rows_ -1))
        { if ((maze_[current_position.row + 1][current_position.column] == '_') || 
            (maze_[current_position.row + 1][current_position.column] == '$')) {isExtensible = true; } }

    // EAST position is extensible if '_' or '$' characters
    else if ((dir == EAST) && (current_position.column < maze_columns_ -1))
        { if ((maze_[current_position.row][current_position.column + 1] == '_') ||
            (maze_[current_position.row][current_position.column + 1] == '$')) {isExtensible = true; } }

    else { isExtensible = false; }

    return isExtensible;
}
