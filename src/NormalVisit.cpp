#include "NormalVisit.h"

const double NormalVisit::NORMAL_FEE = 200.0;

NormalVisit::NormalVisit(const std::string& date, const std::string& doctorName, const std::string& diagnosis)
    : Visit(date, doctorName, diagnosis) {}

double NormalVisit::calculateFee() const {
    return NORMAL_FEE;
}

std::string NormalVisit::getCaseType() const {
    return "NORMAL";
}
