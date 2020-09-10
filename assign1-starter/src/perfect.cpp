// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "timer.h"
using namespace std;

/* This function takes in a number n and calculates the sum
 * of all proper divisors of n, excluding itself.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function is provided a number n and returns a boolean
 * (true/false) value representing whether or not the number is
 * perfect. A perfect number is a non-zero positive number whose
 * sum of its proper divisors is equal to itself.
 */
bool isPerfect (long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function performs an exhaustive search for perfect numbers.
 * It takes as input a number called `stop` and searches for perfect
 * numbers between 1 and `stop`. Any perfect numbers that are found will
 * be printed out to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush;
    }
    cout << "Done searching up to " << stop << endl;
}

/* 求 n 的所有被整除数的和
 * header comment.
 */
long smarterSum(long n){
    if (n <= 1) {
        return 0;
    }
    long total = 1, divisor = 1;
    for (divisor = 2; divisor <= sqrt(n); divisor++) {
        if (n % divisor == 0) {
            if (n / divisor == divisor){
                total += divisor;
            } else {
                total += divisor + n / divisor;
            }
        }
    }
    return total;
}

/* 调用 smarterSum 方法，判断 n 是不是 perfect number
 * header comment.
 */
bool isPerfectSmarter(long n){
    /* TODO: Fill in this function. */
    return (n != 0) && (n == smarterSum(n));
}

/* 调用 isPerfectSmarter 方法，找到 [1, stop) 中的 perfect number
 * header comment.
 */
void findPerfectsSmarter(long stop){
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush;
    }
    cout << "Done searching up to " << stop << endl;
}

// 判断一个数是否为 素数
bool isPrime(int n) {
    // Corner case
    if (n <= 1)
        return false;

    // Check from 2 to n-1
    for (int i = 2; i < n; i++)
        if (n % i == 0)
            return false;

    return true;
}

/* 获取第 N 个 perfect number
 * TODO: 考虑 long 溢出的情况
 * header comment.
 */
long findNthPerfectEuclid(long n){
    /* TODO: Fill in this function. */
    int startK = 0;
    while (n > 0){
        startK++;
        if (isPrime(pow(2, startK) - 1)){
            n -= 1;
        }
    }
    // 2^(k-1) * (2^k - 1)
    if (startK >= 1){
        return pow(2, startK - 1) * (pow(2, startK) - 1);
    }else{
        return -1;
    }
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Time multiple trials of findPerfects function on doubling input sizes."){
    cout << endl;
    TIME_OPERATION(20000, findPerfects(20000));
//    TIME_OPERATION(40000, findPerfects(40000));
//    TIME_OPERATION(80000, findPerfects(80000));
//    TIME_OPERATION(160000, findPerfects(160000));
//    TIME_OPERATION(320000, findPerfects(320000));
//    TIME_OPERATION(640000, findPerfects(640000));
}

PROVIDED_TEST("Check divisor sums of small numbers") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Check 6 and 28, should be perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Imperfect numbers") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Oddballs, zero, one, negative") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
    EXPECT(!isPerfect(-6));
}

PROVIDED_TEST("33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

// TODO: add your test cases here
//// test negative numbers
//STUDENT_TEST("Check negative number is perfect") {
//    EXPECT_EQUAL(isPerfect(-100), false);
//    EXPECT_EQUAL(isPerfect(-1000), false);
//}

//// test wrong except
//STUDENT_TEST("Test wrong except") {
//    EXPECT_EQUAL(isPerfect(5), true);
//}

//// Check smarter sum work right
//STUDENT_TEST("Check smarter sum work right") {
//    // 6 (1, 2, 3)
//    EXPECT_EQUAL(smarterSum(6), 6);
//    // 25 (1, 5)
//    EXPECT_EQUAL(smarterSum(25), 6);
//    EXPECT_EQUAL(smarterSum(0), 0);
//}

//// check perfect smart
//STUDENT_TEST("Check perfect smart."){
//    cout << endl;
//    TIME_OPERATION(20000, findPerfectsSmarter(20000));
//    TIME_OPERATION(40000, findPerfectsSmarter(40000));
//    TIME_OPERATION(80000, findPerfectsSmarter(80000));
//    TIME_OPERATION(160000, findPerfectsSmarter(160000));
//    TIME_OPERATION(320000, findPerfectsSmarter(320000));
//    TIME_OPERATION(640000, findPerfectsSmarter(640000));
//}

//STUDENT_TEST("Check findNthPerfectEuclid"){
//    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
//    EXPECT_EQUAL(findNthPerfectEuclid(7), 137438691328);
//    EXPECT_EQUAL(findNthPerfectEuclid(0), -1);
//    EXPECT_EQUAL(findNthPerfectEuclid(-10), -1);
//}
