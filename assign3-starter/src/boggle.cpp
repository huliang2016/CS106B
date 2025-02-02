/* 
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/* 
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int points(string str) {
    /* TODO: Fill in the remainder of this function. */
    return str.length() <= 3 ? 0:str.length() - 3;
}

void process(GridLocation position, Grid<char>& board, Lexicon& lex, string temp, Grid<bool>& hasBeen, Set<string>& result) {
    if (lex.containsPrefix(temp) || temp.length() == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j=-1; j <= 1; j++) {
                if (i == 0 && j == 0) {
                    continue;
                } else {
                    GridLocation next(position.row + i, position.col + j);
                    if (0 <= next.row && next.row < board.numRows() && 0 <= next.col && next.col < board.numCols() && !hasBeen[next.row][next.col]) {
                        // 合法的下一个坐标
                        hasBeen[next.row][next.col] = true;
                        process(next, board, lex, temp + to_string(board[next.row][next.col]), hasBeen, result);
                        hasBeen[next.row][next.col] = false;
                    }
                }
            }
        }
    }
    if (lex.contains(temp)) {
        result.add(temp);
    }
}

/* 
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /* TODO: Fill in the remainder of this function. */
    Set<string> result;
    for (int i=0; i < board.numRows(); i++) {
        for (int j=0; j < board.numCols(); j++) {
            Grid<bool> hasBeen(board.numRows(), board.numCols());
            GridLocation initPosition(i, j);
            process(initPosition, board, lex, "", hasBeen, result);
        }
    }

    // 计算得分
    int score = 0;
    for (string item : result) {
        score += points(item);
    }
    return score;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
