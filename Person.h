#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Person {
public:
    explicit Person(const string &name);

    string getName() const;

    //function to allow dynamic binding and to check the type of object
    virtual string whoAmI();

protected:
    string name;
};

#endif
