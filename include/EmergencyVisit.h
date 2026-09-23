#ifndef EMERGENCYVISIT_H
#define EMERGENCYVISIT_H

#include "Visit.h"

class EmergencyVisit : public Visit {
private:
    static const double EMERGENCY_FEE; // defined in .cpp

public:
    EmergencyVisit(const std::string& date, const std::string& doctorName, const std::string& diagnosis);

    double calculateFee() const override;
    std::string getCaseType() const override;
};

#endif
