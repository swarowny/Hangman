#ifndef MANAGER_H
#define MANAGER_H

#include "Person.h"
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

class Manager : public Person {
public:
    // constructor initialises name and registration number using arguments
    Manager(const string &name, int regNo);

    // method to return registration number
    int getRegNo() const;

    string whoAmI() override;

private:
    int regNo;
    string name;
};

#endif
