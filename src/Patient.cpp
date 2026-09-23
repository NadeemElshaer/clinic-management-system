#include "Patient.h"
#include <iomanip>

Patient::Patient(int id, const std::string& name, int age, const std::string& phone)
    : Person(id, name), age(age), phone(phone) {}

int Patient::getAge() const {
    return age;
}

std::string Patient::getPhone() const {
    return phone;
}

void Patient::setAge(int newAge) {
    age = newAge;
}

void Patient::setPhone(const std::string& newPhone) {
    phone = newPhone;
}

void Patient::displayInfo() const {
    std::cout << "ID    : " << id << "\n"
              << "Name  : " << name << "\n"
              << "Age   : " << age << "\n"
              << "Phone : " << phone << "\n"
              << "Visits: " << getVisitCount() << "\n";
}

void Patient::addVisit(Visit* visit) {
    history.addVisit(visit);
}

void Patient::printHistory() const {
    history.printHistory(name);
}

double Patient::getTotalPaid() const {
    return history.getTotalPaid();
}

int Patient::getVisitCount() const {
    return history.getCount();
}

std::ostream& operator<<(std::ostream& os, const Patient& p) {
    os << std::left << std::setw(6) << p.id
       << std::setw(16) << p.name
       << std::setw(6) << p.age
       << p.phone;
    return os;
}
