/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;


void process(Vector<int>& blocks, Vector<int>& temp, Vector<Vector<int>>& result, int startIdx, int tempSum, int halfSum) {
    if (tempSum > halfSum) {
        // 每个数都不是有效的投票
        if (tempSum - temp[0] > halfSum) {
            return;
        }
        result.add(temp);
    }
    for (int idx = startIdx; idx < blocks.size(); idx++) {
        temp.push_back(blocks[idx]);
        process(blocks, temp, result, idx + 1, tempSum + blocks[idx], halfSum);
        temp.pop_back();
    }
}


int sum(Vector<int> blocks) {
    int result = 0;
    for (int block: blocks) {
        result += block;
    }
    return result;
}


/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Vector<int> computePowerIndexes(Vector<int>& blocks) {
    Vector<int> result;
    Vector<Vector<int>> processResult;
    Vector<int> temp;
    // 计算 list 的长度
    int allSum = sum(blocks), voteCnt = 0;

    // 从大到小排序
    Vector<int> newBlocks(blocks);
    newBlocks.sort();
    newBlocks.reverse();

    // 遍历所有可能性
    cout << newBlocks << endl;
    process(newBlocks, temp, processResult, 0, 0, allSum / 2);
    cout << "finish travel, processResult size: " << processResult.size() << ", " << pow(2, newBlocks.size());

    Map<int, int> valueCnt;
    for (Vector<int> block: processResult) {
        int tempSum = sum(block);
        for (int num: block){
            if (tempSum - num <= allSum / 2) {
                if (valueCnt.containsKey(num)) {
                    valueCnt[num] += 1;
                } else {
                    valueCnt[num] = 1;
                }
                voteCnt += 1;
            } else {
                break;
            }
        }
    }

    Map<int, int> blockCnt;
    for (int block: blocks) {
        if (blockCnt.containsKey(block)) {
            blockCnt[block] += 1;
        } else {
            blockCnt[block] = 1;
        }
    }

    for (int block: blocks) {
        if (valueCnt.containsKey(block)) {
            result.add(valueCnt[block] * 100 / blockCnt[block] / voteCnt);
        } else {
            result.add(0);
        }
    }

    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

// FIXME(lhu)：速度太慢了，感觉上优化空间只有在 DFS 的时候剪枝
//             目前想到了两种，一种是代码里写到了的
//             还有一种是当 起点之后的和小于全部和的一半时，可以全部剪掉，但是这个加上之后在下面时间测试中，反而更长了，比较奇怪吧。。。
PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    /* This is a stress test, which tests the efficiency of your
     * recursive process. Your solution will need to avoid any expensive
     * copying or ADT operations (insertions/removals) in order to be able
     * to find the solution for this voting block setup.
     *
     * On a standard computer system, an effieicent implementation that avoids
     * unnecessary work will take about 30 seconds - 1 minute to complete this
     * test.
     */
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    /* Note: Due to rounding errors, the following values don't all add up to 100. */
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 22; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}
