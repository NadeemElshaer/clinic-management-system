#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include "Person.h"

class Doctor : public Person {
private:
    std::string specialization;
    double consultationFee;

public:
    Doctor(int id, const std::string& name, const std::string& specialization, double consultationFee);

    std::string getSpecialization() const;
    double getConsultationFee() const;
    void setSpecialization(const std::string& newSpec);
    void setConsultationFee(double newFee);

    void displayInfo() const override;
};

#endif
