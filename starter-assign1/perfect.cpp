/*the "perfect.cpp" file contains functions and test cases related to perfect numbers, and perfect Euclid numbers. We start off with a brute force search for perfecet numbers, and respective benchmarks, and then we implament a Search that is up to the square root of N. We then bench mark that approach, and test a Euclid pefect number approach and benchmark thath
 */
#include "console.h"
#include <iostream>
#include <cmath>
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 1;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: The smarterSum function takes in one argument N and returns a boolean value indicating weather or not n is perfect.
 * this time, we are iterating until the rounded up version ( to account for the edge case where sqrt N is a decimal) of sqrt N, because if something is a divisor of a number,
 * it multiplied by something must equal that number, therefore the thing that its multiplied by is also a divisor.
 * header comment.
 */
long smarterSum(long n) {
    if ( n <= 1) {
        return 0;
    }
    long total = 0;
    long root_n = (long)sqrt(n);
    //Note: sqrt N discards the decimal place, but it is an Integer
    for ( long divisor = 1; divisor <= root_n; divisor++) {
        if (divisor == 1) {
            total++;
        }
        else if (divisor != n) {
            //this accounts for the edge case where sqrt N is close to 1 less than N so it will automatically incrament and then add N

            if (n % divisor == 0){
                total += divisor;
                if (divisor != n/divisor) {
                    total += n/divisor;
                }
            }
        }

    }
    return total;
}

/* The isPerfectSmarter function takes in a long "n" and returns weather N is equal to the smarter sum of N ( so, N must be perfect by definition) and N is not 0.
 * header comment.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}


/* The FindPerfectsSmarter takes ini one argument stop and it prints out each perfect number it finds before the stop number.
 * It calls the isPerfectSmarter function which is the more efficient is perfect function
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {

        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;

}

/* This function takes in a long N and outputs the correstponding perfect euclid number
 *
 * GENERAL DIAGRAM Pasted from assignment:
 *
 * Start by setting k = 1.
Calculate m = 2k-1 (Note: C++ has no exponentiation operator, instead use library function pow)
Determine whether m is prime or composite. (Hint: a prime number has a divisorSum and smarterSum equal to one. Code reuse is your friend!)
If m is prime, then the value 2(k-1) * (2k-1) is a perfect number. If this is the nth perfect number you have found, stop here.
Increment k and repeat from step 2.
 */
long findNthPerfectEuclid(long n) {
    long k = 1;

    long count = 0;
    while (true) {
        long m = pow(2, k) - 1;
        if (smarterSum(m) == 1) {
            if (count == n - 1) {
                return ((long)pow(2, k - 1) * m);
            }
            count++;
        }
        k++;
    }
    return 0;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}


// TODO: add your student test cases here

/*
 * Below is a suggestion of how to use a loop to set the input sizes
 * for a sequence of time trials.
 *
 *
*/
STUDENT_TEST("Multiple time trials of findPerfects on increasing input sizes") {

    int smallest = 1000, largest = 8000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfects(size));
    }
}
STUDENT_TEST("Testing negative numbers on time trials for findPerfects") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-2));
    EXPECT(!isPerfect(-3));
}

STUDENT_TEST("Test cases for smarterSum "){
    EXPECT_EQUAL(smarterSum(-1), 0);
    EXPECT_EQUAL(smarterSum(-36), 0);
    EXPECT_EQUAL(smarterSum(0), 0);

    // 1 has no positive proper divisors.
    EXPECT_EQUAL(smarterSum(1), 0);

    // Primes
    EXPECT_EQUAL(smarterSum(2), 1);
    EXPECT_EQUAL(smarterSum(3), 1);
    EXPECT_EQUAL(smarterSum(17), 1);
    EXPECT_EQUAL(smarterSum(97), 1);

    // Perfect squares, so root should only be counted once
    EXPECT_EQUAL(smarterSum(4), 3);    // 1 + 2
    EXPECT_EQUAL(smarterSum(9), 4);    // 1 + 3
    EXPECT_EQUAL(smarterSum(25), 6);   // 1 + 5
    EXPECT_EQUAL(smarterSum(36), 55);  // 1 + 2 + 3 + 4 + 6 + 9 + 12 + 18
    EXPECT_EQUAL(smarterSum(49), 8);   // 1 + 7

    // Non-integer square roots, because we are looping to a square root.
    EXPECT_EQUAL(smarterSum(8), 7);    // 1 + 2 + 4
    EXPECT_EQUAL(smarterSum(12), 16);  // 1 + 2 + 3 + 4 + 6
    EXPECT_EQUAL(smarterSum(15), 9);   // 1 + 3 + 5
    EXPECT_EQUAL(smarterSum(35), 13);  // 1 + 5 + 7

    // Perfect numbers: proper divisors sum to the number itself.
    EXPECT_EQUAL(smarterSum(6), 6);
    EXPECT_EQUAL(smarterSum(28), 28);
    EXPECT_EQUAL(smarterSum(496), 496);
    EXPECT_EQUAL(smarterSum(8128), 8128);

}
STUDENT_TEST("Test the first and second and third and fourth Euclid number") {
    //Using L to make the numbers longs
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6L);
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28L);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496L);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128L);

}
STUDENT_TEST("Test biggest euclid number in bit limit"){
    EXPECT_EQUAL(findNthPerfectEuclid(8), 2305843008139952128L);
}

STUDENT_TEST("Timing FindPerfectSmarter on different sizes") {

    int smallest = 1000, largest = 8000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfects(size));
    }
}


