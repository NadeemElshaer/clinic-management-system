#include "Person.h"

Person::Person(int id, const std::string& name) : id(id), name(name) {}

Person::~Person() {}

int Person::getId() const {
    return id;
}

std::string Person::getName() const {
    return name;
}

void Person::setName(const std::string& newName) {
    name = newName;
}
