/*
 * File: MarbleBoard.cpp
 * ---------------------
 * TODO: Replace this comment with a description of your program, your name,
 * and your partner's name if you worked as a pair. Cite any sources of help.
 */
#include "MarbleBoard.h"
#include "set.h"
#include "SimpleTest.h"
using namespace std;

/*
 *This function returns a vector of moves that are hte possible moves avaliable to the board at a given board state.
 *we iterate until we find a square with a marble in it, and then look if a move can be made by that marble, we then append that move to a list
 *which we return
 */
Vector<Move> MarbleBoard::findPossibleMoves() const {
    Vector<Move> moves;
    //Create lists for iteration of directions because we shouldnt hardcode
    Vector<int> dr = {-1, 1, 0, 0};
    Vector<int> dc = {0, 0, -1, 1};

    for (int i = 0; i < _grid.numRows(); i++) {
        for (int j = 0; j < _grid.numCols(); j++) {

            if (_grid[i][j] == MARBLE_OCCUPIED) {

                for (int d = 0; d < 4; d++) {
                    //iteration of rdirections
                    int midRow = i + dr[d];
                    int midCol = j + dc[d];

                    int endRow = i + 2 * dr[d];
                    int endCol = j + 2 * dc[d];

                    if (_grid.inBounds(midRow, midCol) && _grid.inBounds(endRow, endCol) && _grid[midRow][midCol] == MARBLE_OCCUPIED && _grid[endRow][endCol] == MARBLE_EMPTY) {

                        Move move = {
                            GridLocation(i, j),
                            GridLocation(endRow, endCol)
                        };

                        moves.add(move);
                    }
                }
            }
        }
    }

    return moves;
}

/*
 * the makeMove function takes in a move and changes the board and upates counts so that that move is made. we compute the middle marble by finding the average
 *
 */
void MarbleBoard::makeMove(Move move) {
    int midRow = (move.start.row + move.end.row) / 2;
    int midCol = (move.start.col + move.end.col) / 2;
    _numMarbles--;
    _grid[move.start.row][move.start.col] = MARBLE_EMPTY;
    _grid[midRow][midCol] = MARBLE_EMPTY;
    _grid[move.end.row][move.end.col] = MARBLE_OCCUPIED;
}

/*
 * The undomove funciton takes in a move and undoes the board, assuming that move was made. we compute the middle marble by finding the average.
 */
void MarbleBoard::undoMove(Move move) {

    int midRow = (move.start.row + move.end.row) / 2;
    int midCol = (move.start.col + move.end.col) / 2;
    _numMarbles++;
    _grid[move.start.row][move.start.col] = MARBLE_OCCUPIED;
    _grid[midRow][midCol] = MARBLE_OCCUPIED;
    _grid[move.end.row][move.end.col] = MARBLE_EMPTY;
}









/* The functions below have already been written for you! */

MarbleBoard::MarbleBoard() : MarbleBoard(fullBoard()) {}   // default constructor creates the starting board

MarbleBoard::MarbleBoard(const Grid<Marble>& grid) {
    _grid = grid;
    _numMarbles = countMarbles(grid);
}

int MarbleBoard::numMarbles() const {
    return _numMarbles;
}

bool MarbleBoard::isSolved() const {
    return _numMarbles == 1;
}

Marble MarbleBoard::at(GridLocation loc) const {
    return _grid[loc];
}

BoardEncoding MarbleBoard::encode() const {
    return compressBoard(_grid);
}

Grid<Marble> MarbleBoard::toGrid() const {
    return _grid;
}

string MarbleBoard::toString() const {
    return boardToString(_grid);
}


/* * * * * * Test Cases * * * * * */

/*
 * Helper for the tests below: returns the moves as a Set, so that the tests
 * do not depend on the order in which findPossibleMoves returns its moves.
 */
static Set<Move> toSet(const Vector<Move>& moves) {
    Set<Move> result;
    for (const Move& move : moves) {
        result.add(move);
    }
    return result;
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: a lone marble has no moves") {
    MarbleBoard board(loadBoard("marbles/boards/solved.txt"));
    EXPECT_EQUAL(board.findPossibleMoves().size(), 0);
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: two marbles side by side can jump either way") {
    MarbleBoard board(boardFromStrings({
        "XX---XX",
        "XX---XX",
        "-------",
        "-OO----",
        "-------",
        "XX---XX",
        "XX---XX"
    }));
    Vector<Move> moves = board.findPossibleMoves();
    Set<Move> expected = { { {3, 1}, {3, 3} },     // left marble jumps right
                           { {3, 2}, {3, 0} } };   // right marble jumps left
    EXPECT_EQUAL(moves.size(), 2);
    EXPECT_EQUAL(toSet(moves), expected);
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: no jumping over empty holes, diagonally, off the board, or into corners") {
    // marbles with empty holes between them
    MarbleBoard spaced(boardFromStrings({
        "XXO-OXX",
        "XX---XX",
        "O-O-O-O",
        "-------",
        "O-O-O-O",
        "XX---XX",
        "XXO-OXX"
    }));
    EXPECT_EQUAL(spaced.findPossibleMoves().size(), 0);

    // marbles touching only diagonally
    MarbleBoard diagonals(loadBoard("marbles/boards/no-diagonals.txt"));
    EXPECT_EQUAL(diagonals.findPossibleMoves().size(), 0);

    // the only jumps in this line of marbles would land off the board or in a corner
    MarbleBoard edge(loadBoard("marbles/boards/off-the-edge.txt"));
    EXPECT_EQUAL(edge.findPossibleMoves().size(), 0);
}

PROVIDED_TEST("MarbleBoard::findPossibleMoves: the full board has exactly four moves, all into the center") {
    MarbleBoard board;
    Vector<Move> moves = board.findPossibleMoves();
    Set<Move> expected = { { {1, 3}, {3, 3} }, { {5, 3}, {3, 3} },
                           { {3, 1}, {3, 3} }, { {3, 5}, {3, 3} } };
    EXPECT_EQUAL(moves.size(), 4);
    EXPECT_EQUAL(toSet(moves), expected);
}

PROVIDED_TEST("MarbleBoard::makeMove and undoMove: jump down into the center, then undo") {
    MarbleBoard board;
    Move move = { {1, 3}, {3, 3} };
    board.makeMove(move);
    EXPECT_EQUAL(board.at({1, 3}), MARBLE_EMPTY);       // jumping marble left its start
    EXPECT_EQUAL(board.at({2, 3}), MARBLE_EMPTY);       // jumped-over marble is gone
    EXPECT_EQUAL(board.at({3, 3}), MARBLE_OCCUPIED);    // jumping marble landed
    EXPECT_EQUAL(board.numMarbles(), 31);

    board.undoMove(move);
    EXPECT_EQUAL(board, MarbleBoard());
    EXPECT_EQUAL(board.numMarbles(), 32);
}

PROVIDED_TEST("MarbleBoard::makeMove and undoMove: jump to the left, then undo") {
    MarbleBoard board(loadBoard("marbles/boards/one-jump.txt"));
    MarbleBoard original = board;
    Move move = { {3, 2}, {3, 0} };
    board.makeMove(move);
    EXPECT_EQUAL(board, MarbleBoard(boardFromStrings({
        "XX---XX",
        "XX---XX",
        "-------",
        "O------",
        "-------",
        "XX---XX",
        "XX---XX"
    })));
    EXPECT(board.isSolved());
    board.undoMove(move);
    EXPECT_EQUAL(board, original);
    EXPECT_EQUAL(board.numMarbles(), 2);
}
