// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include "testing/SimpleTest.h"
#include "map.h"
#include "set.h"
#include <string>
#include <iostream>
#include "filelib.h"
#include <fstream>
#include "simpio.h"
#include "strlib.h"
#include "search.h"
using namespace std;

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string cleanToken(string token) {
    /* TODO: Fill in the remainder of this function. */
    string result = "";
    for (int tokenIdx = 0; tokenIdx < int(token.length()); tokenIdx++) {
        if ('a' <= token.at(tokenIdx) && token.at(tokenIdx) <= 'z') {
            result += to_string(token.at(tokenIdx));
        } else if ('A' <= token.at(tokenIdx) && token.at(tokenIdx) <= 'Z') {
            result += to_string(char(token.at(tokenIdx) + 32));
        }
    }
    return result;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Map<string, Set<string>> readDocs(string dbfile) {
    Map<string, Set<string>> docs;
    /* TODO: Fill in the remainder of this function. */
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);

    if (lines.size() % 2 != 0) {
        // 奇数行，可以丢掉最后一行
        lines.removeBack();
    }

    for (int i = 0; i < lines.size();) {
        string url = lines[i];
        string content = lines[i + 1];
        Vector<string> temp = stringSplit(content, " ", -1);
        Set<string> contents;
        for (int tempIdx = 0; tempIdx < temp.size(); tempIdx++) {
            string tempContent = temp[tempIdx];
            string tempContentFilter = cleanToken(tempContent);
            if (tempContentFilter.length() == 0){
                continue;
            }
            contents.add(tempContentFilter);
        }
        docs.put(url, contents);
        i += 2;
    }
    return docs;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Map<string, Set<string>> buildIndex(Map<string, Set<string>>& docs) {
    Map<string, Set<string>> index;
    /* TODO: Fill in the remainder of this function. */
    for (string key : docs.keys()) {
        for (string temp : docs[key]) {
            if (index.containsKey(temp)) {
                index[temp].add(key);
            } else {
                index[temp] = Set<string>{key};
            }
        }
    }
    return index;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Set<string> result;
    /* TODO: Fill in the remainder of this function. */
    Stack<Set<string>> stack;
    for (string temp : stringSplit(query, " ", -1)) {
        if (temp.size() == 0) {
            continue;
        }

        string key = cleanToken(temp);
        if (stack.size() == 0) {
            if (index.containsKey(key)) {
                stack.push(index[key]);
            }
            continue;
        }

        Set<string> last = stack.pop();
        Set<string> curr;
        if (index.containsKey(key)) {
            curr = index[cleanToken(temp)];
        }

        if (temp.at(0) == '-') {
            stack.push(last - curr);
        } else if (temp.at(0) == '+') {
            stack.push(last * curr);
        } else {
            stack.push(last + curr);
        }
    }

    while (!stack.isEmpty()) {
        result += stack.pop();
    }
    return result;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void searchEngine(string dbfile) {
    /* TODO: Fill in the remainder of this function. */
    Map<string, Set<string>> docs = readDocs(dbfile);
    cout << "Stand by while building index..." << endl;
    Map<string, Set<string>> index = buildIndex(docs);
    cout << "Indexed " << docs.size() << " pages containing " << index.size() << " unique terms." << endl;

    while (true) {
        string input = getLine("Enter query sentence (RETURN/ENTER to quit):");
        if (input.length() == 0) {
            break;
        } else {
            Set<string> result = findQueryMatches(index, input);
            // Found 2 matching pages
            // {"http://cs106b.stanford.edu/assignments/assign2/searchengine.html", "http://cs106b.stanford.edu/qt/troubleshooting.html"}
            cout << "Found " << result.size() << " matching pages" << endl;
            cout << result.toString() << endl;
        }
    }
    cout << endl << "All done!" << endl;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on tokens with no punctuation") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
}

PROVIDED_TEST("cleanToken on tokens with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word tokens"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("readDocs from tiny.txt, contains 4 documents") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    EXPECT_EQUAL(docs.size(), 4);
}

PROVIDED_TEST("readDocs from tiny.txt, suess has 5 unique words and includes lowercase fish") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Set<string> seuss = docs["www.dr.seuss.net"];
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 20 unique tokens overall") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    cout << "=========== " + index.toString() << endl;

    Set<string> matchesRed = findQueryMatches(index, "red");
    cout << matchesRed << endl;
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));

    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    cout << matchesHippo << endl;
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
