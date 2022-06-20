#include "Person.h"
#include <iostream>

Person::Person(const string &name) {
    this->name = name;
}

string Person::getName() const {
    return name;
}

string Person::whoAmI() {
    return "person";
}