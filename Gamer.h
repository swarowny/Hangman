#ifndef GAMER_H
#define GAMER_H

#include "Person.h"
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>

using namespace std;

class NoScoreException : public exception {
public:
    explicit NoScoreException(const char *msg) noexcept: msg(msg) {}

    const char *what() const noexcept override {
        return msg;
    }

private:
    const char *msg;

};

class Gamer : public Person {
public:
    static const int EASY;
    static const int MEDIUM;
    static const int HARD;
    // initialises name and registration number using arguments
    Gamer(const string &name, int regNo);

    // adds the score to the map
    void addScore(int lvl, float score);

    // updates the cumulative score map
    // throws NoScoreException if gamer has no score yet
    bool updateCumulativeScore(int lvl);

    // calculates average scores
    void calculateAverageScore(int lvl);

    // updates all scores
    void updateAllScores();

    // finds maximum scores
    void calculateMaxScores();

    // clears users' scores
    void clearScores();

    string whoAmI() override;

    // returns registration number
    int getRegNo() const;

    // returns scores
    multimap<int, float> getScores();

    float getCumulativeScores(int lvl);

    float getAverageScores(int lvl);

    float getMaxScores(int lvl);

    // methods used for sorting to display top 10
    static bool sortByEasyScore(Gamer* a, Gamer* b);

    static bool sortByMedScore(Gamer* a, Gamer* b);

    static bool sortByHardScore(Gamer* a, Gamer* b);

private:
    int regNo;
    string name;
    multimap<int, float> scores;  // scores map, with each in range 0.0 to 100.0, and an
    //associated level of difficulty
    map<int, float> cumulativeScores;  // scores map
    //associated level of difficulty
    map<int, float> averageScores;  // scores map
    //associated level of difficulty
    map<int, float> maxScores;

    // friend function to output details of gamer to stream
    // should output name, regno, and minimum, maximum and average score on a single line
    // if the gamer has no scores "has no scores" should be output instead of the scores
    friend ostream &operator<<(ostream &str, const Gamer &g);
};

#endif
