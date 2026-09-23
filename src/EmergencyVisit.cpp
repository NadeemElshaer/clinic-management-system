#include "EmergencyVisit.h"

const double EmergencyVisit::EMERGENCY_FEE = 400.0;

EmergencyVisit::EmergencyVisit(const std::string& date, const std::string& doctorName, const std::string& diagnosis)
    : Visit(date, doctorName, diagnosis) {}

double EmergencyVisit::calculateFee() const {
    return EMERGENCY_FEE;
}

std::string EmergencyVisit::getCaseType() const {
    return "EMERGENCY";
}
