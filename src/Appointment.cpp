#include "Appointment.h"

Appointment::Appointment(int patientId, int doctorId, const std::string& date, const std::string& time)
    : patientId(patientId), doctorId(doctorId), date(date), time(time) {}

int Appointment::getPatientId() const {
    return patientId;
}

int Appointment::getDoctorId() const {
    return doctorId;
}

std::string Appointment::getDate() const {
    return date;
}

std::string Appointment::getTime() const {
    return time;
}
