#include "Doctor.h"
#include <iostream>

Doctor::Doctor(int id, const std::string& name, const std::string& specialization, double consultationFee)
    : Person(id, name), specialization(specialization), consultationFee(consultationFee) {}

std::string Doctor::getSpecialization() const {
    return specialization;
}

double Doctor::getConsultationFee() const {
    return consultationFee;
}

void Doctor::setSpecialization(const std::string& newSpec) {
    specialization = newSpec;
}

void Doctor::setConsultationFee(double newFee) {
    consultationFee = newFee;
}

void Doctor::displayInfo() const {
    std::cout << "ID   : " << id << "\n"
              << "Name : Dr. " << name << "\n"
              << "Spec : " << specialization << "\n"
              << "Fee  : " << consultationFee << " EGP\n";
}
