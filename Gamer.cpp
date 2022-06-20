#include "Gamer.h"
#include <iomanip>
#include <cmath>
#include <vector>

Gamer::Gamer(const string &name, int regNo) : Person(name), regNo(regNo) {}

const int Gamer::EASY = 0;
const int Gamer::MEDIUM = 1;
const int Gamer::HARD = 2;

int Gamer::getRegNo() const {
    return regNo;
}

multimap<int, float> Gamer::getScores() {
    return scores;
}

// adds score to the scores map
void Gamer::addScore(int lvl, float score) {
    scores.insert(make_pair(lvl, score));
}

// updates the cumulative score for each lvl
bool Gamer::updateCumulativeScore(int lvl) {
    float cumulativeScore = 0;
    try {
        if (scores.count(lvl) == 0)
            throw NoScoreException("User has no scores");
        else {
            for (auto score: scores) {
                if (score.first == lvl) {
                    cumulativeScore += score.second;
                }
            }
            cumulativeScores[lvl] = cumulativeScore;
        }
    }
    catch (NoScoreException &ex) {
        return false;
    }
    return true;
}

// calculates average scores for each lvl
void Gamer::calculateAverageScore(int lvl) {
    float numOfScores = scores.count(lvl);
    if (numOfScores == 0) return;
    float avgScore = cumulativeScores[lvl] / numOfScores;

    if (averageScores.count(lvl) == 0) {
        averageScores.insert(make_pair(lvl, avgScore));
    } else {
        averageScores[lvl] = avgScore;
    }
}

void Gamer::updateAllScores() {
    updateCumulativeScore(EASY);
    updateCumulativeScore(MEDIUM);
    updateCumulativeScore(HARD);
    calculateAverageScore(EASY);
    calculateAverageScore(MEDIUM);
    calculateAverageScore(HARD);
}

void Gamer::calculateMaxScores() {
    float maxEasy = 0, maxMed = 0, maxHard = 0;
    maxScores.clear();

    for (auto score: scores) {
        if (score.first == EASY) {
            if (maxEasy < score.second)
                maxEasy = score.second;
        } else if (score.first == MEDIUM) {
            if (maxMed < score.second)
                maxMed = score.second;
        } else if (score.first == HARD) {
            if (maxHard < score.second)
                maxHard = score.second;
        }
    }
    maxScores.insert(make_pair(EASY, maxEasy));
    maxScores.insert(make_pair(MEDIUM, maxMed));
    maxScores.insert(make_pair(HARD, maxHard));
}

void Gamer::clearScores() {
    scores.clear();
}

string Gamer::whoAmI() {
    return "gamer";
}

float Gamer::getCumulativeScores(int lvl) {
    return cumulativeScores[lvl];
}

float Gamer::getAverageScores(int lvl) {
    return averageScores[lvl];
}

float Gamer::getMaxScores(int lvl) {
    return maxScores[lvl];
}

// outputs gamer details
ostream &operator<<(ostream &str, const Gamer &g) {
    str << g.getRegNo() << ", " << g.getName();
    if (!g.scores.empty()) {
        float min = 101.0, max = -1.0;
        for (auto score: g.scores) {
            if (score.second < min)
                min = score.second;
            if (score.second > max)
                max = score.second;
        }

        float sum = 0;
        float numOfScores = 0;
        for (auto avgScore: g.averageScores) {
            if (!isnan(avgScore.second)) {
                sum += avgScore.second;
                numOfScores++;
            }
        }
        str << ", min: " << min << ", max: " << max << ", avg: " << fixed << setprecision(2) << sum / numOfScores
            << endl;
    } else {
        str << " has no scores" << endl;
    }
    return str;
}

bool Gamer::sortByEasyScore(Gamer *a, Gamer *b) {
    return a->maxScores[EASY] > b->maxScores[EASY];
}

bool Gamer::sortByMedScore(Gamer *a, Gamer *b) {
    return a->maxScores[MEDIUM] > b->maxScores[MEDIUM];
}

bool Gamer::sortByHardScore(Gamer *a, Gamer *b) {
    return a->maxScores[HARD] > b->maxScores[HARD];
}
