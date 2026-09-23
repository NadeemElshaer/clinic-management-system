#ifndef NORMALVISIT_H
#define NORMALVISIT_H

#include "Visit.h"

class NormalVisit : public Visit {
private:
    static const double NORMAL_FEE; // defined in .cpp

public:
    NormalVisit(const std::string& date, const std::string& doctorName, const std::string& diagnosis);

    double calculateFee() const override;
    std::string getCaseType() const override;
};

#endif
