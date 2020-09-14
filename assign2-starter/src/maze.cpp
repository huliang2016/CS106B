// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "set.h"
#include "maze.h"
#include "mazegraphics.h"
#include "testing/SimpleTest.h"
using namespace std;


/* 对每个路径生成下一个合法路径
 * header comment.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    Set<GridLocation> candidateLocations = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for(auto it = candidateLocations.begin(); it != candidateLocations.end(); it++){
        int row = cur.row + it->row;
        int col = cur.col + it->col;
        if (0 <= row && row < maze.numRows() && 0 <= col && col < maze.numCols() && maze[row][col]){
            neighbors.add(GridLocation(row, col));
        }
    }
    return neighbors;
}


/* 检查结果是否合法
 * header comment.
 */
void checkSolution(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeStart = {0, 0};
    // path size should be greater than 0
    if (path.isEmpty()) {
        error("Path size should be greater than 0");
    }

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }

    Set<GridLocation> alreadyBeen;
    GridLocation lastLocation = {-1, -1}, location;
    while (!path.isEmpty()){
        location = path.pop();
        if (location.row < 0 || location.row >= maze.numRows() || location.col < 0 || location.col >= maze.numCols()) {
            error("Location not valid, " + location.toString());
        }
        if (!maze[location.row][location.col]){
            error("Location is a wall" + location.toString());
        }
        if (alreadyBeen.contains(location)) {
            error("Location is duplicated" + location.toString());
        } else {
            alreadyBeen.add(location);
        }
        if (lastLocation.row != -1 && abs(location.col - lastLocation.col) + abs(location.row - lastLocation.row) != 1){
            error("Location teleports, last location: " + lastLocation.toString() + ", next location: " + location.toString());
        }
        lastLocation = location;
    }

    if (location != mazeStart){
        error("Path does not start at maze start");
    }

    /* If you find a problem with the solution, call error() to report it.
     * If the path is valid, then this function should run to completion
     * without throwing any errors.
     */
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;

    // 复制一个 maze 使用用
    Grid<bool> tempMaze(maze);

    // 初始化起点
    GridLocation startLocation = {0, 0};
    path.add(startLocation);
    if (!tempMaze[startLocation.row][startLocation.col]) {
        error("can not find a path");
    }
    tempMaze[startLocation.row][startLocation.col] = false;

    // 初始化终点
    GridLocation endLocation = {tempMaze.numRows() - 1, tempMaze.numCols() - 1};
    if (!tempMaze[endLocation.row][endLocation.col]) {
        error("can not find a path");
    }

    // 初始化 bfs 参数
    Queue<Stack<GridLocation>> candidatePaths;
    Queue<Grid<bool>> tempMazes;
    candidatePaths.enqueue(path);
    tempMazes.enqueue(tempMaze);

    // bfs
    while (!candidatePaths.isEmpty()) {
        int size = candidatePaths.size();
        while (size-- > 0) {
            Stack<GridLocation> candidate = candidatePaths.dequeue();
            Grid<bool> tempMaze = tempMazes.dequeue();
            if (!candidate.isEmpty()){
                if (candidate.peek() == endLocation) {
                    try {
                        checkSolution(maze, candidate);
                        MazeGraphics::highlightPath(candidate, "GREEN");
                        return candidate;
                    } catch (exception e) {
                        continue;
                    }
                }
                Set<GridLocation> nextMoves = generateValidMoves(tempMaze, candidate.peek());
                for (auto it = nextMoves.begin(); it != nextMoves.end(); it++) {
                    Stack<GridLocation> temp(candidate);
                    Grid<bool> temp2(tempMaze);
                    temp.push(*it);
                    temp2[it->row][it->col] = false;
                    candidatePaths.enqueue(temp);
                    tempMazes.enqueue(temp2);
                }
            }
        }
    }
    error("can not find a path");
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * However, there are various malformed inputs which it does not
 * correctly handle. At the very end of this assignment, you will
 * update this code to have complete error-checking behavior to make
 * the function fully robust. Then, update this header comment to
 * indicate which error checking cases you added. See the writeup
 * for more details.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following lines read in the data from the file into a Vector
     * of strings representing the lines of the file. We haven't talked
     * in class about what ifstreams are, so don't worry if you don't fully
     * understand what is going on here.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    for (int idx = 1; idx < numRows; idx++) {
        if (int(lines[idx].length()) != numCols) {
            error("not same length, this line length: " + to_string(lines[idx].length()) + ", expect length: " + to_string(numCols));
        }
    }
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("maze file contains unknow char: " + to_string(ch));
            }
        }
    }
}

/* This provided functions opens and read the contents of files ending
 * in a .soln extension and interprets the contents as a Stack of
 * GridLocations, populating the provided soln data structure.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    Vector<string> lines;
    readEntireFile(in, lines);

    if (lines.size() != 1){
        error("File contained too many or too few lines.");
    }

    istringstream istr(lines[0]); // Stack read does its own error-checking
    if (!(istr >> soln)) {// if not successfully read
        error("Solution did not have the correct format.");
    }
}

bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateNeighbors on location in the center of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> neighbors = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

PROVIDED_TEST("generateNeighbors on location on the side of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> neighbors = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, side));
}

PROVIDED_TEST("generateNeighbors on corner of 2x2 grid with walls"){
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {{1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for medium maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for large maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}


PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
}


PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("Test readMazeFile on valid file 2x2.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/2x2.maze", maze));
}

PROVIDED_TEST("Test readMazeFile on valid file 5x7.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/5x7.maze", maze));
}

PROVIDED_TEST("readMazeFile on nonexistent file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/nonexistent_file", g));
}

PROVIDED_TEST("readMazeFile on malformed file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/malformed.maze", g));
}

STUDENT_TEST("test all input files") {
    Vector<string> files;
    listDirectory("res/", files);

    Vector<string> filterFiles;
    for (int idx = 0; idx < files.size(); idx++){
        if (ends_with(files[idx], "malformed.maze")){
            Grid<bool> g;
            EXPECT_ERROR(readMazeFile("res/malformed.maze", g));
        } else if (ends_with(files[idx], ".maze")) {
            string filePath = "res/" + files[idx];
            cout << "trying to solve " + filePath << endl;

            Grid<bool> maze;
            readMazeFile(filePath, maze);
//            filePath = filePath.replace(filePath.rfind(".maze"), filePath.length(), ".soln");

            // 存在解
            Stack<GridLocation> soln = solveMaze(maze);
            EXPECT_NO_ERROR(checkSolution(maze, soln));
        }
    }
}
