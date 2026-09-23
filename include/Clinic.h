#ifndef CLINIC_H
#define CLINIC_H

#include <string>
#include "Patient.h"
#include "Doctor.h"
#include "WaitingRoom.h"
#include "Appointment.h"

// Clinic ties everything together: the patient/doctor records, the
// waiting room, and the console menu. This is the "main logic" class.
class Clinic {
private:
    static const int MAX_PATIENTS = 200;
    static const int MAX_DOCTORS = 50;
    static const int MAX_APPOINTMENTS = 200;

    Patient* patients[MAX_PATIENTS]; // always kept sorted by ID -> enables binary search
    int patientCount;

    Doctor* doctors[MAX_DOCTORS];
    int doctorCount;

    Appointment* appointments[MAX_APPOINTMENTS];
    int appointmentCount;

    WaitingRoom waitingRoom; // priority queue, built from scratch

    // --- core algorithms ---
    int binarySearchById(int id, int& comparisons) const;
    Doctor* findDoctorById(int id) const;
    void sortPatients(); // selection sort by name or age, counts comparisons
    bool isDoctorBooked(int doctorId, const std::string& date, const std::string& time) const;

    // --- sub-menus ---
    void patientsMenu();
    void doctorsMenu();
    void waitingRoomMenu();
    void appointmentsMenu();

    // --- patient operations ---
    void addPatient();
    void viewAllPatients() const;
    void searchPatientById() const;
    void deletePatient();
    void viewPatientHistory() const;

    // --- doctor operations ---
    void addDoctor();
    void viewAllDoctors() const;

    // --- waiting room operations ---
    void addToWaitingRoom();
    void viewWaitingRoom() const;
    void callNextPatient();

    // --- appointment operations (bonus feature) ---
    void bookAppointment();
    void viewAllAppointments() const;

    // --- safe console input helpers: the program must never crash on bad input ---
    static int readInt(const std::string& prompt);
    static std::string readLine(const std::string& prompt);
    static std::string currentTimeString();
    static std::string currentDateString();

public:
    Clinic();
    ~Clinic(); // frees every Patient* and Doctor* it allocated

    void run(); // the main menu loop
};

#endif
