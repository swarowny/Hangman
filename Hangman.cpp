#include "Hangman.h"
#include "ReadWords.h"
#include "WordProcessor.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

Hangman::Hangman() = default;

Hangman::Hangman(const char *filename, const int *levels) : filename(filename) {
    this->levels[0] = levels[0];
    this->levels[1] = levels[1];
    this->levels[2] = levels[2];
}

int Hangman::lastManagerRegNo;

void Hangman::loadUsers(fstream &file, const char *fname) {
    file.open(fname, ios::in);
    if (!file) {
        cout << "Failed to open " << fname << endl;
        exit(1);
    }

    vector<string> lines;
    vector<string> attributes;
    vector<string> levelScores;
    string line;
    int lvl;
    Manager *m;
    Gamer *g;
    int lastMRegNo = 0;

    // loading all lines from the file to a vector and closing the file
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    file.clear();

    // depending on regNo, creating a manager or gamer object
    // adding objects to corresponding sets and adding scores to gamer objects
    for (string s: lines) {
        WordProcessor::split(s, attributes, ',');
        if (attributes[0].length() == 4) {
            m = new Manager(attributes[1], stoi(attributes[0]));
            managers.push_back(m);
            if (stoi(attributes[0]) > lastMRegNo)
                lastMRegNo = stoi(attributes[0]);
        } else {
            g = new Gamer(attributes[1], stoi(attributes[0]));
            gamers.push_back(g);
            for (int i = 2; i < attributes.size(); i++) {
                if (i == 2)
                    lvl = Gamer::EASY;
                else if (i == 3)
                    lvl = Gamer::MEDIUM;
                else if (i == 4)
                    lvl = Gamer::HARD;
                WordProcessor::split(attributes[i], levelScores, '/');
                for (const string &score: levelScores) {
                    g->addScore(lvl, stof(score));
                }
                levelScores.clear();
            }
        }
        attributes.clear();
    }
    lastManagerRegNo = lastMRegNo;
}

void Hangman::loadConfig(fstream &file, const char *fname) {
    file.open(fname, ios::in);
    if (!file) {
        cout << "Failed to open " << fname << endl;
        exit(1);
    }

    string line;
    vector<string> config;

    getline(file, line);
    file.close();
    file.clear();
    WordProcessor::split(line, config, ',');

    // setting the class members 
    // (words file and minimum values of word lengths for each level)
    filename = config[0].c_str();
    levels[0] = stoi(config[1]);
    levels[1] = stoi(config[2]);
    levels[2] = stoi(config[3]);
}

void Hangman::saveUsers(fstream &file, const char *fname) {
    file.open(fname, ios::out);
    if (!file) {
        cout << "Failed to save users in " << fname << endl;
        exit(1);
    }

    for (Manager *m: managers) {
        file << setw(4) << setfill('0') << m->getRegNo() << "," << m->getName() << endl;
    }
    for (Gamer *g: gamers) {
        multimap<int, float> scores = g->getScores();
        file << g->getRegNo() << "," << g->getName();
        if (!scores.empty()) {
            bool first = true;
            file << ",";
            for (auto score: scores) {
                if (score.first == Gamer::EASY) {
                    if (first) {
                        file << score.second;
                        first = false;
                    } else
                        file << "/" << score.second;
                }
            }
            first = true;
            file << ",";
            for (auto score: scores) {
                if (score.first == Gamer::MEDIUM) {
                    if (first) {
                        file << score.second;
                        first = false;
                    } else
                        file << "/" << score.second;
                }
            }
            first = true;
            file << ",";
            for (auto score: scores) {
                if (score.first == Gamer::HARD) {
                    if (first) {
                        file << score.second;
                        first = false;
                    } else
                        file << "/" << score.second;
                }
            }
        }
        file << endl;
    }
    file.close();
    file.clear();
}

void Hangman::saveConfig(fstream &file, const char *fname) {
    file.open(fname, ios::out);
    if (!file) {
        cout << "Failed to save configuration in " << fname << endl;
        exit(1);
    }
    file << filename << "," << levels[0] << "," << levels[1] << "," << levels[2] << endl;
    file.close();
    file.clear();
}

void Hangman::showMenu(string &user) {
    if (user == "gamer") {
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<
             "\n\t1) Play game" <<
             "\n\t2) Your statistics" <<
             "\n\t3) Display top 10" <<
             "\n\t4) Exit game" <<
             "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
    } else {
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<
             "\n\t1) Reset game" <<
             "\n\t2) Change game levels" <<
             "\n\t3) Add another manager" <<
             "\n\t4) Set the word file" <<
             "\n\t5) Exit game" <<
             "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
    }
}

void Hangman::showLevels() {
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<
         "\n\t1) Easy -> word length " << levels[0] << "-" << levels[1] - 1 <<
         "\n\t2) Medium -> word length " << levels[1] << "-" << levels[2] - 1 <<
         "\n\t3) Hard -> word length " << levels[2] << "+" <<
         "\n\t4) Go back" <<
         "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
}

void Hangman::playGame(int lvl, Gamer *g) {
    ReadWords r(filename, *this);
    r.setLevel(lvl);
    r.getWords();
//    ReadWords::getPhrases();
    int badGuesses = 0;
    float score = 0;
    guesses.clear();

    while (true) {
        won = true;
        string coveredWord;

        //example word
        wordToGuess = "astonishing";

        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << setw(33) << right << "AVAILABLE LETTERS";
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        printLetters('A', 'M');
        printLetters('N', 'Z');
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

        // print the covered word
        for (char c: WordProcessor::toUpper(wordToGuess)) {
            if (guesses.find(c) == string::npos) {
                won = false;
                coveredWord += "_ ";
            } else {
                coveredWord += c;
                coveredWord += "";
            }
        }

        if (won) {
            cout << "Congrats, " << g->getName() << ", you won!!" << endl;
            score += (10 - badGuesses) * 5;
            cout << "You have scored " << score << " points!";
            g->addScore(lvl, score);
            break;
        }

        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << setw(27) << right << "GUESS" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << coveredWord << endl;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

        // taking a guess from gamer
        char guess;
        cout << "Take a guess: ";
        cin >> guess;
        while (!isalpha(guess)) {
            cout << "Pick a letter: ";
            cin >> guess;
        }
        while (guesses.find(toupper(guess)) != string::npos) {
            cout << "You have already tried this one. Pick another: ";
            cin >> guess;
        }
        guesses.push_back(toupper(guess));
        if (wordToGuess.find(guess) == string::npos) {
            cout << endl << "Wrong letter. ";
            cout << "You have another " << 10 - (++badGuesses) << " attempts." << endl;
            if (badGuesses == 10) {
                cout << endl << "Sorry. You lost." << endl;
                break;
            }
        } else {
            cout << endl << "Good! +5 points" << endl;
            score += 5;
        }

    }
    r.close();
}

void Hangman::printLetters(char from, char to) {
    cout << setw(12) << right << " ";
    for (char i = from; i <= to; i++) {
        if (guesses.find(i) == string::npos)
            cout << i << " ";
        else
            cout << "  ";
    }
    cout << endl;
}

void Hangman::displayStatistics(Gamer *g) {
    g->updateAllScores();
    g->calculateMaxScores();

    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << setw(30) << right << "STATISTICS" <<
         "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << right << setw(9) << " " << setw(12) << "PER GAME" << setw(16) << "ACCUMULATED" << setw(12) << "AVERAGE"
         << endl;
    cout << left << fixed << setprecision(2) << setw(9) << "EASY" << right << setw(12) << g->getMaxScores(Gamer::EASY)
         << setw(16) << g->getCumulativeScores(Gamer::EASY) << setw(12) << g->getAverageScores(Gamer::EASY) << endl;
    cout << left << setw(9) << "MEDIUM" << right << setw(12) << g->getMaxScores(Gamer::MEDIUM) << setw(16)
         << g->getCumulativeScores(Gamer::MEDIUM) << setw(12) << g->getAverageScores(Gamer::MEDIUM) << endl;
    cout << left << setw(9) << "HARD" << right << setw(12) << g->getMaxScores(Gamer::HARD) << setw(16)
         << g->getCumulativeScores(Gamer::HARD) << setw(12) << g->getAverageScores(Gamer::HARD) << endl;
}

void Hangman::displayTop10() {

    for (Gamer *g: gamers) {
        g->updateAllScores();
        g->calculateMaxScores();
    }

    // display top 10
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << setw(28) << right << "TOP 10" <<
         "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << setw(27) << right << "EASY" << endl;
    sort(gamers.begin(), gamers.end(), Gamer::sortByEasyScore);
    for (int i = 0; i < 10; i++) {
        cout << setw(15) << "" << i + 1 << ". " << gamers[i]->getName() << " " << gamers[i]->getMaxScores(Gamer::EASY)
             << endl;
    }
    cout << endl << setw(28) << right << "MEDIUM" << endl;
    sort(gamers.begin(), gamers.end(), Gamer::sortByMedScore);
    for (int i = 0; i < 10; i++) {
        cout << setw(15) << "" << i + 1 << ". " << gamers[i]->getName() << " " << gamers[i]->getMaxScores(Gamer::MEDIUM)
             << endl;
    }
    cout << endl << setw(27) << right << "HARD" << endl;
    sort(gamers.begin(), gamers.end(), Gamer::sortByHardScore);
    for (int i = 0; i < 10; i++) {
        cout << setw(15) << "" << i + 1 << ". " << gamers[i]->getName() << " " << gamers[i]->getMaxScores(Gamer::HARD)
             << endl;
    }
}

void Hangman::resetGame() {
    setWordFile("..//sample.txt");
    changeLevels(0, 1);
    changeLevels(1, 6);
    changeLevels(2, 10);
    for (Gamer *g: gamers) {
        g->clearScores();
    }
}

void Hangman::changeLevels(int lvl, int min) {
    if (lvl == 0) {
        levels[0] = min;
        if (levels[1] - 1 <= levels[0]) levels[1] = levels[0] + 2;
        if (levels[2] - 1 <= levels[1]) levels[2] = levels[1] + 2;
    } else if (lvl == 1) {
        levels[1] = min;
        if (levels[2] - 1 <= levels[1]) levels[2] = levels[1] + 2;
        if (levels[0] >= levels[1]) levels[0] = (levels[1] - 2 > 0) ? levels[1] - 2 : 1;
    } else {
        levels[2] = min;
        if (levels[1] >= levels[2]) levels[1] = levels[2] - 2;
        if (levels[0] >= levels[1]) levels[0] = (levels[1] - 2 > 0) ? levels[1] - 2 : 1;
    }
}

void Hangman::addGamer(Gamer *g) {
    gamers.push_back(g);
}

void Hangman::addManager(string &username) {
    auto *m = new Manager(username, lastManagerRegNo + 1);
    managers.push_back(m);
    lastManagerRegNo++;
}

void Hangman::setWordFile(const char *fname) {
    filename = fname;
}

pair<int, int> Hangman::getLevels(int lvl) {
    return make_pair(levels[lvl], levels[lvl + 1] - 1);
}

const char *Hangman::getFilename() const {
    return filename;
}

vector<Gamer *> Hangman::getGamers() const {
    return gamers;
}

vector<Manager *> Hangman::getManagers() const {
    return managers;
}

