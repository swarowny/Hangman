#include "Manager.h"
#include <iostream>

Manager::Manager(const string &name, int regNo) : Person(name), regNo(regNo) {}

int Manager::getRegNo() const {
    return regNo;
}

string Manager::whoAmI() {
    return "manager";
}