#include "ReadWords.h"
#include "WordProcessor.h"
#include "Hangman.h"
#include <iostream>
#include <utility>

// constructor opens a file and initialises members
ReadWords::ReadWords(const char *fname, Hangman h) : hangman(move(h)) {
    wordFile.open(fname);
    if (!wordFile) {
        cout << "Failed to open " << fname << endl;
        exit(1);
    }
    eofFound = false;
}

// returns next word in a file, converted to lower case
// uses a one-word lookahead to avoid any problems relating to when if end-of-file
// is detected due to absence/presence of newline at end of file
string ReadWords::getNextWord() {
    wordFile >> nextWord;

    if (wordFile.eof())
        eofFound = true;

    nextWord = WordProcessor::removePunct(nextWord);

    // checks if word contains a letter,
    // if no - returns empty string, if yes - converts to lower case
    if (WordProcessor::containsLetter(nextWord)) {
        nextWord = WordProcessor::toLower(nextWord);
    } else
        nextWord = "";

    return nextWord;
}

//TODO maybe look for the word, then go back to a dot, take all the phrase till next dot including
bool ReadWords::getPhrases() {
    return true;
}

//TODO also random words -> select random number, go there and check if before is a space, 
// if not, go to space and then take a word
// get words depending on difficulty
bool ReadWords::getWords() {

    for (int i = 0; i < 5; i++) {

        if (!eofFound) {
            nextWord = getNextWord();
        } else return false;
        while (nextWord.length() < hangman.getLevels(level).first || nextWord.length() > hangman.getLevels(level).second) {
            if (!eofFound) nextWord = getNextWord();
            else return false;
        }

        words.insert(nextWord);
    }
    return true;
}

// closes the file
void ReadWords::close() {
    wordFile.close();
    wordFile.clear();
}

void ReadWords::setLevel(int lvl) {
    level = lvl;
}

