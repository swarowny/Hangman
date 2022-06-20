#ifndef WORDPROCESSOR_H
#define WORDPROCESSOR_H

#include <string>
#include <vector>

using namespace std;

// an additional class to process words
class WordProcessor {
public:
    // returns a copy of the word with all leading and trailing punctuation removed
    static string removePunct(string word);

    // returns a copy of the word in lower case
    static string toLower(string word);

    // returns a copy of the word in upper case
    static string toUpper(string word);

    // checks if there is a letter in a word
    static bool containsLetter(string &word);

    // splits string into a vector
    static void split(string &line, vector<string> &v, char delimiter);
};

#endif