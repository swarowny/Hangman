#include "Person.h"
#include "Manager.h"
#include "Gamer.h"
#include "WordProcessor.h"
#include "Hangman.h"
#include <sstream>
#include <limits>

// checks if the registration number for the gamer is valid
// (if it has 5 digits and no leading zeros, and if it is not taken)
bool isRegNoValid(int &regNo, fstream &file, const char *fname) {
    if (regNo < 10000 || regNo > 99999)
        return false;

    file.open(fname, ios::in);
    if (!file) {
        cout << "Failed to open " << fname << endl;
        exit(1);
    } else {
        string line, curRegNo;
        while (getline(file, line)) {
            istringstream ss(line);
            while (ss) {
                getline(ss, curRegNo, ',');
                ss.ignore(numeric_limits<streamsize>::max());
                if (to_string(regNo) == curRegNo) {
                    file.close();
                    file.clear();
                    return false;
                }
            }
        }
        file.close();
        file.clear();
        return true;
    }
}

// checks if user exists in file
bool userExists(string &username, fstream &file, const char *fname) {
    // opening file and exiting if file cannot be opened
    file.open(fname, ios::in);
    if (!file) {
        cout << "Failed to open " << fname << endl;
        exit(1);
    }

    // looking for user in the file
    string line, curName;

    while (getline(file, line)) {
        istringstream ss(line);
        while (ss) {
            ss.ignore(numeric_limits<streamsize>::max(), ',');
            getline(ss, curName, ',');
            ss.ignore(numeric_limits<streamsize>::max(), '\n');
            if (WordProcessor::toLower(curName) == WordProcessor::toLower(username)) {
                file.close();
                file.clear();
                return true;
            }
        }
    }
    file.close();
    file.clear();
    return false;
}

// checks if user is a gamer or manager, returns a new gamer/manager object
// creates a new gamer if user does not exist
template<class T>
T login(string &username, fstream &file, const char *fname, Hangman &h) {
    while (true) {
        for (Manager *m: h.getManagers()) {
            if (WordProcessor::toLower(m->getName()) == WordProcessor::toLower(username)) {
                return m;
            }
        }

        for (Gamer *g: h.getGamers()) {
            if (WordProcessor::toLower(g->getName()) == WordProcessor::toLower(username)) {
                return g;
            }
        }

        // making sure user wants to create a new gamer
        // and a different name was not just a spelling mistake
        int tryAgain;
        cout << "No such user. Do you want to create a new gamer? " <<
             "\n\t1 - Yes" <<
             "\n\t2 - No" << endl;
        cin >> tryAgain;
        while (!cin.good() || (tryAgain != 1 && tryAgain != 2)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Type 1 for yes or 2 for no" << endl;
            cin >> tryAgain;
        }
        if (tryAgain == 2) {
            cout << "Enter your username: ";
            cin.ignore();
            getline(cin >> ws, username);
            continue;
        }

        // creating and returning a new gamer
        int newRegNo;

        cout << "Choose your registration number (5 digit number): " << endl << endl;
        cin >> newRegNo;
        while (!cin.good() || !isRegNoValid(newRegNo, file, fname)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Choose a different registration number (10000-99999): " << endl;
            cin >> newRegNo;
        }
        auto *g = new Gamer(username, newRegNo);
        h.addGamer(g);
        cout << "\nNew gamer created: " << username << ", " << newRegNo << endl;

        return g;
    }
}


int main() {
    fstream file;
    const char *usersFile = "..//users.txt";
    const char *configFile = "..//config.txt";
    const char *wordsFile = "..//sample.txt";
    const int lvls[] = {1,5,10};

    //TODO: somehow loadConfig messes up the filename

    Hangman h(wordsFile, lvls);
//    Hangman h;
    h.loadUsers(file, usersFile);

    // asking for username and logging in
    string name;
    cout << "Enter your username: ";
    getline(cin >> ws, name);
    auto *p = login<Person *>(name, file, usersFile, h);
    Gamer *g = nullptr;
    string user = p->whoAmI();

    if (p->whoAmI() == "gamer") {
        g = dynamic_cast<Gamer *> (p);
    }
    else delete g;

    h.loadConfig(file, configFile);
    string choice;

    while (true) {
        Hangman::showMenu(user);
        cout << "Choice: ";
        cin >> choice;

        if (user == "gamer" && choice == "1") {
            string levelChoice;
            while (true) {
                h.showLevels();
                cout << "Choice: ";
                cin >> levelChoice;

                if (levelChoice == "1") {
                    h.playGame(Gamer::EASY, g);
                    break;
                } else if (levelChoice == "2") {
                    h.playGame(Gamer::MEDIUM, g);
                    break;
                } else if (levelChoice == "3") {
                    h.playGame(Gamer::HARD, g);
                    break;
                } else if (levelChoice == "4") {
                    break;
                } else {
                    cout << "Please choose option 1-4" << endl << endl;
                }
            }
        } else if (user == "gamer" && choice == "2") {
            Hangman::displayStatistics(g);
        } else if (user == "gamer" && choice == "3") {
            h.displayTop10();
        } else if (user == "manager" && choice == "1") {
            h.resetGame();
        } else if (user == "manager" && choice == "2") {
            int level;
            int min;
            cout << "\nWhich level do you wish to change? ";
            h.showLevels();

            cin >> level;
            while (!cin.good() || (level != 1 && level != 2 && level != 3 && level != 4)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Choose level 1-3 or 4 to go back: " << endl;
                cin >> level;
            }
            if (level == 4) {
                continue;
            }

            cout << "\nChoose the lower bound: ";
            cin >> min;
            while (!cin.good() || (level == 1 && min < 1) || (level == 2 && min < 3) || (level == 3 && min < 5)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cin.good()) cout << "Choose a higher number: " << endl;
                else cout << "Choose the lower bound: " << endl;
                cin >> min;
            }

            h.changeLevels(level - 1, min);
            cout << endl << "Levels changed: ";
            h.showLevels();
        } else if (user == "manager" && choice == "3") {
            string username;
            cout << "Enter the manager's username or 1 to go back: ";
            getline(cin >> ws, username);
            if (username == "1") {
                continue;
            }
            while (userExists(username, file, usersFile)) {
                cout << "Username is already taken. Please choose a different one: ";
                getline(cin >> ws, username);
            }
            h.addManager(username);
        } else if (user == "manager" && choice == "4") {
            string filename;
            while (true) {
                cout << "Current filename: " << h.getFilename() << endl;
                cout << "Enter the filename or press 1 to go back: ";
                cin >> filename;
                if (filename == "1") {
                    break;
                }
                while ((filename).find(".txt") == string::npos) {
                    cout << "Please enter correct filename ending with \".txt\"" << endl;
                    cin >> filename;
                }
                string correct;
                cout << "Are you sure \"" << filename << "\" is the correct filename?" <<
                     "\n\t1) Yes" <<
                     "\n\t2) No" << endl;
                cin >> correct;
                while (correct != "1" && correct != "2") {
                    cout << "Please type 1 for yes or 2 for no: ";
                    cin >> correct;
                }
                if (correct == "2") {
                    continue;
                } else {
                    h.setWordFile(filename.c_str());
                    cout << "\nFilename successfully changed to " << filename << endl;
                    break;
                }
            }
            continue;
        } else if ((user == "gamer" && choice == "4") || (user == "manager" && choice == "5")) {
            cout << "Bye!" << endl << endl;
            break;
        } else {
            cout << "Please choose option 1-";
            cout << (user == "gamer" ? "4" : "5") << endl << endl;
        }
    }
    h.saveConfig(file, configFile);
    h.saveUsers(file, usersFile);
    delete g;

    return 0;
}


//TODO
/* ********** FOR LECTURER ***********
I could not finish the assignment because when I run getWords() method,
it somehow changes my filename, and I have no idea how to fix it
*/

