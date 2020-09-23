/* 
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "error.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This 
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/* 
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
void drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* TODO: Fill in the remainder of this function. */
    if (order < 0) {
        error("order should be greater or equal to 0");
    } else if (order == 0) {
        fillBlackTriangle(window, one, two, three);
    } else {
        GPoint one_two((one.getX() + two.getX())/ 2, (one.getY() + two.getY())/ 2);
        GPoint one_three((one.getX() + three.getX())/ 2, (one.getY() + three.getY())/ 2);
        GPoint two_three((two.getX() + three.getX())/ 2, (two.getY() + three.getY())/ 2);

        drawSierpinskiTriangle(window, one, one_two, one_three, order - 1);
        drawSierpinskiTriangle(window, one_two, two, two_three, order - 1);
        drawSierpinskiTriangle(window, one_three, two_three, three, order - 1);
    }
}

/* * * * * * Test Cases * * * * * */

/* 
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo. 
 */

void runDemos();

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runDemos();
}

