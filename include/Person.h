#ifndef PERSON_H
#define PERSON_H

#include <string>

// Abstract base class for anyone in the clinic (Patient, Doctor).
class Person {
protected:
    int id;
    std::string name;

public:
    Person(int id, const std::string& name);
    virtual ~Person(); // virtual destructor so derived destructors run correctly

    int getId() const;
    std::string getName() const;
    void setName(const std::string& newName);

    // Pure virtual function -> makes Person an abstract class.
    virtual void displayInfo() const = 0;
};

#endif
