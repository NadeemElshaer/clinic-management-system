#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>

// A simple booking record: a patient, a doctor, and a date/time slot.
class Appointment {
private:
    int patientId;
    int doctorId;
    std::string date; // e.g. "2026-09-10"
    std::string time; // e.g. "3:00 PM"

public:
    Appointment(int patientId, int doctorId, const std::string& date, const std::string& time);

    int getPatientId() const;
    int getDoctorId() const;
    std::string getDate() const;
    std::string getTime() const;
};

#endif
