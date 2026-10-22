/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "SimpleTest.h"
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
 * the drawSierpinskiTriangle function returns an int because it creates a tree where for each serpinski triangle it creates, it creates 3 of order n -1 at its corners and midpoints, with the basecase being 0
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {

    if (order <= 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    GPoint Middle_1_2 = GPoint((two.x + one.x)/2, (two.y + one.y)/2);
    GPoint Middle_2_3 = GPoint((three.x + two.x)/2, (three.y + two.y)/2);
    GPoint Middle_3_1 = GPoint((three.x + one.x)/2, (three.y + one.y)/2);
    return drawSierpinskiTriangle(window, one,Middle_1_2, Middle_3_1, order - 1) + drawSierpinskiTriangle(window, two, Middle_2_3, Middle_1_2, order - 1) + drawSierpinskiTriangle(window, three, Middle_2_3, Middle_3_1, order -1);
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all your testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

STUDENT_TEST("Sierpinski triangle counts for orders 0 through 5") {
    GWindow window;

    GPoint one(200, 50);
    GPoint two(50, 300);
    GPoint three(350, 300);

    EXPECT_EQUAL(drawSierpinskiTriangle(window, one, two, three, 0), 1);
    EXPECT_EQUAL(drawSierpinskiTriangle(window, one, two, three, 1), 3);
    EXPECT_EQUAL(drawSierpinskiTriangle(window, one, two, three, 2), 9);
    EXPECT_EQUAL(drawSierpinskiTriangle(window, one, two, three, 3), 27);
    EXPECT_EQUAL(drawSierpinskiTriangle(window, one, two, three, 4), 81);
    EXPECT_EQUAL(drawSierpinskiTriangle(window, one, two, three, 5), 243);
}

STUDENT_TEST("Sierpinski with different triangle coordinates") {
    GWindow window;

    GPoint one(100, 100);
    GPoint two(400, 150);
    GPoint three(250, 400);

    EXPECT_EQUAL(drawSierpinskiTriangle(window, one, two, three, 1), 3);
    EXPECT_EQUAL(drawSierpinskiTriangle(window, one, two, three, 2), 9);
    EXPECT_EQUAL(drawSierpinskiTriangle(window, one, two, three, 3), 27);
}