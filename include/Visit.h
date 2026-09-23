#ifndef VISIT_H
#define VISIT_H

#include <string>

// Abstract base class for a single clinic visit.
// EmergencyVisit and NormalVisit override calculateFee() differently -
// this is the polymorphism requirement.
class Visit {
protected:
    std::string date;
    std::string doctorName;
    std::string diagnosis;

public:
    Visit(const std::string& date, const std::string& doctorName, const std::string& diagnosis);
    virtual ~Visit();

    std::string getDate() const;
    std::string getDoctorName() const;
    std::string getDiagnosis() const;

    // Pure virtual -> Visit is abstract. Each child computes its own fee.
    virtual double calculateFee() const = 0;
    // Pure virtual -> lets us label the visit type (EMERGENCY / NORMAL) when printing.
    virtual std::string getCaseType() const = 0;
};

#endif
