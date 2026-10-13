// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


/*
 The generateValidMoves function creates four "direction" variables, left, right up and down. it checks if each variable  is obstructed or out of bounds
and it returns the set of "direction" variables that are within bounds and non obstructed
It takes in a boolean grid (maze) passed by reference, and a the GridLocation cur and it outputs a set of GridLocations*/
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors = {};
    GridLocation left; GridLocation right; GridLocation up; GridLocation down;
    //Account for edge case where cur is obstructed
    if (maze[cur] == false) {
        return {};
    }
    if (cur.col > 0){
        left.col = cur.col - 1;
    }
    if (cur.col < maze.numCols()-1){
        right.col = cur.col + 1;
    }
    if (cur.row > 0) {
        down.row = cur.row - 1;
    }
    if (cur.row < maze.numRows()-1) {
        up.row = cur.row + 1;
    }
    down.col = cur.col, up.col = cur.col, left.row = cur.row, right.row = cur.row;
    if (maze[left] == true && cur.col > 0) {
        neighbors.add(left);
    }
    if (maze[right] == true && cur.col < maze.numCols()-1) {
        neighbors.add(right);
    }
    if (maze[up] == true && cur.row < maze.numRows()-1) {
        neighbors.add(up);
    }
    if (maze[down] == true && cur.row > 0) {
        neighbors.add(down);
    }

    return neighbors;
}

// The "validatePath function takes in a passed by value grid of booleans(the maze) and a vector of gridlocations called path. It computes if a given path is valid for a given maze. if it is not it returns an error
// it checks if path is empty, if the start point is not at 0,0, if the end is not at the end ( maxX-1, MaxY-1), if any of the points on the path are obstructed, or if any moves are invalid. If any of these cases are true
// than the respective error is returned.
void validatePath(Grid<bool>& maze, Vector<GridLocation>& path) {
    if (path.isEmpty()) {
        error("Path is empty!");
    }
    if (path[0].row != 0 || path[0].col != 0 ) {
        error("Start point is not at (0,0)");
    }
    if (path[path.size() - 1].row != (maze.numRows()-1)  || path[path.size() - 1].col != (maze.numCols()-1) ) {
        error("End point not at end of Maze");
    }

    for (int i = 0; i < path.size() - 1; i++) {
        //check if path is obstructed
        if (maze[path[i]] == false) {
            error("point along path is obstructed!");
        }
        if (generateValidMoves(maze, path[i]).contains(path[i+1]) == false && (i < path.size() - 2)) {
            error("The path contains an invalid move");
        }
    }
}



// the solveMazeBfs function takes in a maze(grid) of booleans which are obstructious and uses a queue to implament a breadth first search
//it returns the path, if it exists, of a soloution to the maze. BFS keeps track of the visited paths, iterates through the queue, chekcing if each entry ends at a soloution. If not, it looks at possible
//paths and adds those to the queue
// behavior of the function and how you implemented this behavior
Vector<GridLocation> solveMazeBFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Queue<Vector<GridLocation>> allPaths;
    Vector<GridLocation> startPath = {GridLocation(0,0)};
    GridLocation exit = GridLocation(maze.numRows() -1, maze.numCols()-1);
    allPaths.enqueue(startPath);
    Set<GridLocation> visited;
    visited.add(GridLocation(0, 0));
    while (allPaths.size() != 0) {
        Vector<GridLocation> currentPath = allPaths.dequeue();
        if (currentPath[currentPath.size()-1] == exit) {
            return currentPath;
        }

        if (generateValidMoves(maze, currentPath[currentPath.size()-1]).size() != 0) {
            for (GridLocation nextLocation: generateValidMoves(maze, currentPath[currentPath.size()-1])) {
                if (!visited.contains(nextLocation)) {
                    visited.add(nextLocation);

                    Vector<GridLocation> tempPath = currentPath;
                    tempPath.add(nextLocation);
                    allPaths.enqueue(tempPath);
                }
            }
        }
    }
    return {};





    drawMaze(maze);
    /* TODO: Fill in the remainder of this function. */
    return path;
}

/* the solveMazeDFS function takes in a maze of booleans which are obstructions and uses a Stack to implament a depth first search
 * it returns the path, if it exists, of a soloutino to the maze. DFS keeps track of the visited paths, and iterates through the stack, checking if each entry ends at a soloution, if not it looks at
 * possible moves, and adds those paths to the stack
 */
Vector<GridLocation> solveMazeDFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Stack<Vector<GridLocation>> allPaths;
    Vector<GridLocation> startPath = {GridLocation(0,0)};
    GridLocation exit = GridLocation(maze.numRows() -1, maze.numCols()-1);
    allPaths.push(startPath);
    Set<GridLocation> visited;
    visited.add(GridLocation(0, 0));
    while (allPaths.size() != 0) {
        Vector<GridLocation> currentPath = allPaths.pop();
        if (currentPath[currentPath.size()-1] == exit) {
            return currentPath;
        }

        if (generateValidMoves(maze, currentPath[currentPath.size()-1]).size() != 0) {
            for (GridLocation nextLocation: generateValidMoves(maze, currentPath[currentPath.size()-1])) {
                if (!visited.contains(nextLocation)) {
                    visited.add(nextLocation);
                    Vector<GridLocation> tempPath = currentPath;
                    tempPath.add(nextLocation);
                    allPaths.push(tempPath);
                }
            }
        }
    }
    return {};

}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines = stringSplit(readEntire(in), '\n');

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Vector<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}


PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}


PROVIDED_TEST("validatePath on correct solution read from file, medium maze") {
    Grid<bool> maze;
    Vector<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true},
                       {true, true}};
    Vector<GridLocation> notBeginAtEntry = { {1, 1}, {2, 1} };
    Vector<GridLocation> notEndAtExit = { {0, 0}, {1, 0}, {2, 0} };
    Vector<GridLocation> moveThroughWall = { {0 ,0}, {0, 1}, {1, 1}, {2, 1} };

    EXPECT_ERROR(validatePath(maze, notBeginAtEntry));
    EXPECT_ERROR(validatePath(maze, notEndAtExit));
    EXPECT_ERROR(validatePath(maze, moveThroughWall));
}
//TESTING ON PROVIDED TEST FILES
PROVIDED_TEST("solveMazeBFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMazeDFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 2x2") {
    Grid<bool> maze;
    readMazeFile("res/2x2.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 13x39") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 17x37") {
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}


STUDENT_TEST("solveMazeBFS on file 21x25") {
    Grid<bool> maze;
    readMazeFile("res/21x25.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 25x15") {
    Grid<bool> maze;
    readMazeFile("res/25x15.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 25x33") {
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeBFS on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
PROVIDED_TEST("solveMazeDFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMazeDFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 2x2") {
    Grid<bool> maze;
    readMazeFile("res/2x2.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 13x39") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 17x37") {
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}


STUDENT_TEST("solveMazeDFS on file 21x25") {
    Grid<bool> maze;
    readMazeFile("res/21x25.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 25x15") {
    Grid<bool> maze;
    readMazeFile("res/25x15.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 25x33") {
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMazeDFS on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
// TODO: add your test cases here
//STUDENT TESTS
STUDENT_TEST("generateValidMoves on 3x3 grid center with obstruction"){

    Grid<bool> maze = {{true, false, true},
                       {false, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
STUDENT_TEST("generateValidMoves on grid with no possible moves") {
    Grid<bool> maze = {{true, false, true},
                       {false, true, false},
                       {true, false, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
STUDENT_TEST("generateValidMoves on grid where starting position is obstructed") {
    Grid<bool> maze = {{true, false, true},
                       {false, false, true},
                       {true, false, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
//
STUDENT_TEST("validatePath accepts a valid winding path") {
    Grid<bool> maze = {
        {true,  true,  false},
        {false, true,  false},
        {false, true,  true}
    };
    Vector<GridLocation> path = {
        {0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 2}
    };

    EXPECT_NO_ERROR(validatePath(maze, path));
}

STUDENT_TEST("validatePath rejects an empty path") {
    Grid<bool> maze = {{true}};
    Vector<GridLocation> path;

    EXPECT_ERROR(validatePath(maze, path));
}

STUDENT_TEST("validatePath throws error on incorrect start or end") {
    Grid<bool> maze = {
        {true, true},
        {true, true}
    };
    Vector<GridLocation> wrongStart = {{0, 1}, {1, 1}};
    Vector<GridLocation> wrongEnd = {{0, 0}, {0, 1}};

    EXPECT_ERROR(validatePath(maze, wrongStart));
    EXPECT_ERROR(validatePath(maze, wrongEnd));
}

STUDENT_TEST("validatePath rejects a path through a wall") {
    Grid<bool> maze = {
        {true, false},
        {true, true}
    };
    Vector<GridLocation> path = {{0, 0}, {0, 1}, {1, 1}};

    EXPECT_ERROR(validatePath(maze, path));
}


STUDENT_TEST("BFS solves a single square maze") {
    Grid<bool> maze = {{true}};
    Vector<GridLocation> expected = {{0, 0}};

    EXPECT_EQUAL(solveMazeBFS(maze), expected);

}
STUDENT_TEST("BFS finds correct path ( only one soloution in this set)") {
    Grid<bool> maze = {
        {true,  true,  false},
        {false, true,  false},
        {false, true,  true}
    };
    Vector<GridLocation> expected = {
        {0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 2}
    };

    EXPECT_EQUAL(solveMazeBFS(maze), expected);
}
STUDENT_TEST("BFS finds path in maze with loops/open maze") {
    Grid<bool> maze = {
        {true, true, true},
        {true, true, true},
        {true, true, true}
    };

    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
    EXPECT_EQUAL(soln.size(), 5);
}
STUDENT_TEST("BFS returns empty when there is no way to exit"){
    Grid<bool> maze = {
        {true,  true,  false},
        {true,  true,  false},
        {false, false, true}
    };

    EXPECT(solveMazeBFS(maze).isEmpty());
}
PROVIDED_TEST("validatePath on correct solution, hand-constructed maze") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Vector<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
STUDENT_TEST("DFS solves a single square maze") {
    Grid<bool> maze = {{true}};
    Vector<GridLocation> expected = {{0, 0}};

    EXPECT_EQUAL(solveMazeDFS(maze), expected);

}
STUDENT_TEST("DFS finds correct path ( only one soloution in this set)") {
    Grid<bool> maze = {
        {true,  true,  false},
        {false, true,  false},
        {false, true,  true}
    };
    Vector<GridLocation> expected = {
        {0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 2}
    };

    EXPECT_EQUAL(solveMazeDFS(maze), expected);
}
STUDENT_TEST("DFS finds path in maze with loops/open maze") {
    Grid<bool> maze = {
        {true, true, true},
        {true, true, true},
        {true, true, true}
    };

    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
    EXPECT_EQUAL(soln.size(), 5);
}
STUDENT_TEST("DFS returns empty with no way to exit"){
    Grid<bool> maze = {
        {true,  true,  false},
        {true,  true,  false},
        {false, false, true}
    };

    EXPECT(solveMazeDFS(maze).isEmpty());
}
