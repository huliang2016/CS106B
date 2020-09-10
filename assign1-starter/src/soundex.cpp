// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include <exception>
#include <map.h>
using namespace std;

/* This function takes a string s and returns a version of
 * the string that has all non-letter characters removed
 * from it.
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isalpha(s[i])) {
            // remove the character at index i
            s = s.substr(0,i) + s.substr(i+1);
            i--;
        }
    }
    return s;
}

// string encode
string encodeToNumber(string s){
    string result = "";
    vector<string> stringEncode = {"AEIOUHWY", "BFPV", "CGJKQSXZ", "DT", "L", "MN", "R",};

    for (int i = 0; i < s.length(); i++) {
        int start = 0;
        for(; start < stringEncode.size(); start++){
            if (stringEncode[start].find(toupper(s[i])) != -1){
                break;
            }
        }
        if (start == stringEncode.size()) {
            throw "not valid string input, string: " + s;
        }
        result += to_string(start);
    }
    return result;
}

// remove adjacent duplicate numbers
string removeAdjacentDuplicate(string s){
    for (int i = 1; i < s.length(); i++) {
        if (s[i] == s[i - 1]) {
            // remove the character at index i
            s = s.substr(0, i) + s.substr(i+1);
            i--;
        }
    }
    return s;
}

// upper first letter
string upperFirstLetter(string origin, string encodeNum){
    if (origin.length() == 0) {
        return "";
    }
    string result = "";
    result += toupper(origin[0]);
    result += encodeNum.substr(1, encodeNum.length());
    return result;
}

// remove all zero
string removeAllZeros(string s){
    return stringReplace(s, "0", "");
}

// padding zero
string paddingZeros(string s){
    int size = 4 - s.length();
    while(size-- > 0){
        s += "0";
    }
    return s.substr(0, s.length() > 4? 4:s.length());
}

/* implement soundex algorithm
 * header comment.
 */
string soundex(string s) {
    string encode = removeNonLetters(s);
    encode = encodeToNumber(encode);
    encode = removeAdjacentDuplicate(encode);
    encode = upperFirstLetter(s, encode);
    encode = removeAllZeros(encode);
    encode = paddingZeros(encode);
    return encode;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name 
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames
    
    /* TODO: Fill in the remainder of this function. */
    Map<string, Vector<string>> encodeToNames;
    for (auto it = databaseNames.begin(); it != databaseNames.end(); ++it) {
        string encode = soundex(*it);
        if (!encodeToNames.containsKey(encode)){
            Vector<string> temp;
            temp.add(move(*it));
            encodeToNames.put(encode, temp);
        } else {
             Vector<string> temp = encodeToNames.get(encode);
             temp.add(move(*it));
             encodeToNames.put(encode, temp);
        }
    }

    Vector<string> keySets = encodeToNames.keys();
    for (auto it=keySets.begin(); it != keySets.end(); ++it){
        Vector<string> temp = encodeToNames.get(*it);
        temp.sort();
    }

    string userEnterName;
    while ((userEnterName = getLine("Enter a surname (RETURN to quit)")) != "\n" && userEnterName.length() > 0) {
        string encode = soundex(userEnterName);
        cout << "Soundex code is " << encode << endl;
        Vector<string> temp = encodeToNames.get(encode);
        if (temp.size() > 0){
            cout << "Matches from database: " << temp << endl;
        } else {
            cout << "No Matches from database." << endl;
        }
    }
    cout << "All done!" << endl;
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");

    s = "asd  asd";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "asdasd");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Master"), "M236");
    EXPECT_EQUAL(soundex("Jue"), "J000");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");

    s = "asd  asd";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "asdasd");
}

STUDENT_TEST("Check encodeToNumber"){
    //      0         1         2       3     4     5    6
    // "AEIOUHWY", "BFPV", "CGJKQSXZ", "DT", "L", "MN", "R"
    EXPECT_EQUAL(encodeToNumber("master"), "502306");
    EXPECT_EQUAL(encodeToNumber("ABCDLMR"), "0123456");
    EXPECT_EQUAL(encodeToNumber(""), "");
}

STUDENT_TEST("Check removeAdjacentDuplicate"){
    EXPECT_EQUAL(removeAdjacentDuplicate("0000"), "0");
    EXPECT_EQUAL(removeAdjacentDuplicate("0123"), "0123");
    EXPECT_EQUAL(removeAdjacentDuplicate("00552211"), "0521");
    EXPECT_EQUAL(removeAdjacentDuplicate("0"), "0");
}

STUDENT_TEST("Check remove_all_zero"){
    EXPECT_EQUAL(removeAllZeros(""), "");
    EXPECT_EQUAL(removeAllZeros("0"), "");
    EXPECT_EQUAL(removeAllZeros("0000"), "");
    EXPECT_EQUAL(removeAllZeros("00011"), "11");
}

STUDENT_TEST("Check paddingZeros"){
    EXPECT_EQUAL(paddingZeros("0"), "0000");
    EXPECT_EQUAL(paddingZeros("1"), "1000");
    EXPECT_EQUAL(paddingZeros("123"), "1230");
    EXPECT_EQUAL(paddingZeros("000"), "0000");
    EXPECT_EQUAL(paddingZeros("1234567"), "1234");
}
