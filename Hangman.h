#ifndef HANGMAN_H
#define HANGMAN_H

#include "Manager.h"
#include "Gamer.h"
#include <string>
#include <set>

using namespace std;

class Hangman {
public:

    Hangman();

    Hangman(const char *filename, const int *levels);

// loads all the game users from a file
    void loadUsers(fstream &file, const char *filename);

    // loads the game configurations from a file
    void loadConfig(fstream &file, const char *filename);

    // saves the users and gamers' scores to a file
    void saveUsers(fstream &file, const char *filename);

    // saves the game configurations to a file
    void saveConfig(fstream &file, const char *filename);

    // displays menu to users
    static void showMenu(string &user);

    // displays levels to gamers
    void showLevels();

    // creates a ReadWords file, gets words, phrases and starts the game
    void playGame(int lvl, Gamer *g);

    // prints available letters to gamer
    void printLetters(char from, char to);

    // displays statistics to gamers
    static void displayStatistics(Gamer *g);

    // displays top 10 users
    void displayTop10();

    // resets all configurations and deletes users' scores
    void resetGame();

    // changes lower bounds of word lengths for each level
    void changeLevels(int lvl, int min);

    // adds new gamer to a collection
    void addGamer(Gamer *g);

    // adds new manager to a collection
    void addManager(string &username);

    // sets the words source file
    void setWordFile(const char *filename);

    // returns word lengths for each level
    pair<int, int> getLevels(int lvl);

    // returns words filename
    const char *getFilename() const;

    // returns gamers vector
    vector<Gamer *> getGamers() const;

    // returns managers vector
    vector<Manager *> getManagers() const;

private:
    const char *filename;
    vector<Gamer *> gamers;
    vector<Manager *> managers;
    int levels[3]; //lower bound of word lengths for each level
    string wordToGuess; //one from ReadWords words set
    string guesses;
    bool won;
    static int lastManagerRegNo; //holds the regNo of last registered manager

};

#endif