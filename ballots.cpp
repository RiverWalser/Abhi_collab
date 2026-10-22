/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "recursion.h"
#include "SimpleTest.h"
using namespace std;

/*
The countAllOrderings function returns an int and takes in two integers. It is a recursive function. it creates trees where it subtracks from a and then b at each node, when the count of a or b is 0, then it will return 1
and the sum will go back up the chain so the final number returned will be a count of all ordferings
 */
int countAllOrderings(int a, int b) {
    if (a == 0) {
        return 1;
    }
    if (b == 0) {
        return 1;
    }

    return countAllOrderings(a, b-1) + countAllOrderings(a-1, b);
}

/*
 * The CountGoodOrderings_recursive function similarly creates a tree, however it keeps track of the lead that A has after a  ballot is counted, once the lead is less than 0, the tree stops splitting as we return 0
 *
 */

int countGoodOrderings_recursive(int a, int b, int lead) {
    int total = 0;
    if (lead <= 0) {
        return 0;
    }
    if (a == 0 && b == 0) {
        return 1;
    }
    if (a > 0) {
        total += countGoodOrderings_recursive(a - 1, b, lead + 1);
    }
    if (b > 0) {
        total += countGoodOrderings_recursive(a, b-1, lead - 1);
    }
    return total;
}
int countGoodOrderings(int a, int b) {
    return countGoodOrderings_recursive( a-1, b, 1);
}
/*
 * The CountGoodOrderings function chooses a ballot that we will designnate the lead to be with respect to, in this case a, and then it calls a recursive function that has a third argument, lead.
 */

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("countAllOrderings, two A one B") {
   EXPECT_EQUAL(countAllOrderings(2, 1), 3);
}

PROVIDED_TEST("countGoodOrderings, two A one B") {
   EXPECT_EQUAL(countGoodOrderings(2, 1), 1);
}
STUDENT_TEST("CountGoodOrderings test cases") {
    EXPECT_EQUAL(countGoodOrderings(3, 2), 2);
    EXPECT_EQUAL(countGoodOrderings(4, 2), 5);
    EXPECT_EQUAL(countGoodOrderings(4, 3), 5);
    EXPECT_EQUAL(countGoodOrderings(5, 3), 14);
}
STUDENT_TEST("CountAllOrderings on cases with only one candidate") {
    EXPECT_EQUAL(countAllOrderings(0,0), 1);
    EXPECT_EQUAL(countAllOrderings(1,0), 1);
    EXPECT_EQUAL(countAllOrderings(0,1), 1);
    EXPECT_EQUAL(countAllOrderings(5,0), 1);
    EXPECT_EQUAL(countAllOrderings(0,5), 1);

}
STUDENT_TEST("CountAllOrderings on larger numbers") {
    EXPECT_EQUAL(countAllOrderings(3,2), 10);
    EXPECT_EQUAL(countAllOrderings(5,3), 56);
}