#include "WordProcessor.h"
#include <algorithm>
#include <sstream>
#include <iostream>

string WordProcessor::removePunct(string word) {
    word.erase(word.find_last_not_of("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~") + 1, string::npos);
    word.erase(0, min(word.find_first_not_of("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"), word.length() - 1));
    return word;
}

string WordProcessor::toLower(string word) {
    for (char & i : word) {
        if (isupper(i))
            i = tolower(i);
    }
    return word;
}

string WordProcessor::toUpper(string word) {
    for (char & i : word) {
        if (islower(i))
            i = toupper(i);
    }
    return word;
}

bool WordProcessor::containsLetter(string &word) {
    if (find_if(word.begin(), word.end(), [](char c) { return isalpha(c); }) != word.end())
        return true;
    else
        return false;
}

void WordProcessor::split(string &line, vector<string> &v, char delimiter) {
    istringstream ss(line);
    string s;
    while (getline(ss, s, delimiter)) {
        v.push_back(s);
    }
}