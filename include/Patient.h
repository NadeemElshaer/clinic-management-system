#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <iostream>
#include "Person.h"
#include "VisitHistory.h"
#include "Visit.h"

class Patient : public Person {
private:
    int age;
    std::string phone;
    VisitHistory history; // linked list, built from scratch

public:
    Patient(int id, const std::string& name, int age, const std::string& phone);

    int getAge() const;
    std::string getPhone() const;
    void setAge(int newAge);
    void setPhone(const std::string& newPhone);

    void displayInfo() const override; // required by Person (abstract base)

    void addVisit(Visit* visit);
    void printHistory() const;
    double getTotalPaid() const;
    int getVisitCount() const;

    // Operator overload requirement: lets us do `std::cout << patient;`
    // to print one formatted row (ID, name, age, phone).
    friend std::ostream& operator<<(std::ostream& os, const Patient& p);
};

#endif
