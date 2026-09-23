#include "Visit.h"

Visit::Visit(const std::string& date, const std::string& doctorName, const std::string& diagnosis)
    : date(date), doctorName(doctorName), diagnosis(diagnosis) {}

Visit::~Visit() {}

std::string Visit::getDate() const {
    return date;
}

std::string Visit::getDoctorName() const {
    return doctorName;
}

std::string Visit::getDiagnosis() const {
    return diagnosis;
}
