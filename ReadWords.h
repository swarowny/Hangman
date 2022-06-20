/**
 * ReadWords Interface for CE221 assignment 2
 */

#ifndef READWORDS_H
#define READWORDS_H
/**
 * ReadWords class. Provides mechanisms to read a text file, and return
 * lower-case versions of words from that file.
 */
#include "Hangman.h"
#include <string>
#include <fstream>
#include <set>

using namespace std;

class ReadWords {
public:

    /**
     * Constructor. Opens the file with the given filename and associates in
     * with the wordfile stream.
     * Initialises the other members.
     * Prints an error message then terminates the program if the file cannot be opened.
     * Must not perform any other processing
     * @param fname - a C string naming the file to read.
     */
    explicit ReadWords(const char *fname, Hangman h);

    /**
     * Closes the file.
     */
    void close();

    /**
     * Returns a string, being the next word in the file.
     * All letters should be converted to lower case
     * Leading and trailing punctuation symbols should not be included in the word
     * but punctuation elsewhere should not be removed
     * Returns an empty string if next word contains no letters
     * @return - string - next word.
     */
    string getNextWord();

    /**
     * This method should get all phrases based on the words randomly selected before
     * the return type is bool so that true means the function was successfully executed
     * and false otherwise. Phrases should be stored in the phrases set.
     **/

    static bool getPhrases();

    /**
     * The method getWords should select randomly five words and store them in the words
     * private member array. The return type is bool so that true means the function was successfully
     * executed and false otherwise
     * */
    bool getWords();

    // method used to set the level in order to look for length-appropriate words in getWords()
    void setLevel(int lvl);

private:
    Hangman hangman;
    ifstream wordFile;
    string nextWord;
    bool eofFound;
    int level;
    set<string> words;
    set<string> phrases;

};

#endif
