#include "Clinic.h"
#include "EmergencyVisit.h"
#include "NormalVisit.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <algorithm>
#include <cstdlib>

// ---------------------------------------------------------------------
// construction / destruction
// ---------------------------------------------------------------------

Clinic::Clinic() : patientCount(0), doctorCount(0), appointmentCount(0) {}

Clinic::~Clinic() {
    for (int i = 0; i < patientCount; i++) {
        delete patients[i];
    }
    for (int i = 0; i < doctorCount; i++) {
        delete doctors[i];
    }
    for (int i = 0; i < appointmentCount; i++) {
        delete appointments[i];
    }
}

// ---------------------------------------------------------------------
// safe input helpers -> the program must never crash on bad input
// ---------------------------------------------------------------------

int Clinic::readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.eof()) {
            // Input stream closed (e.g. piped input ran out) - exit cleanly
            // instead of spinning forever.
            std::cout << "\n[X] No more input. Exiting.\n";
            std::exit(0);
        }
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[X] Please enter a whole number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

std::string Clinic::readLine(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    std::getline(std::cin, line);
    return line;
}

std::string Clinic::currentTimeString() {
    time_t t = time(nullptr);
    tm* localTm = localtime(&t);
    char buf[16];
    strftime(buf, sizeof(buf), "%I:%M %p", localTm);
    std::string result(buf);
    if (!result.empty() && result[0] == '0') {
        result = result.substr(1); // "01:05 PM" -> "1:05 PM"
    }
    return result;
}

std::string Clinic::currentDateString() {
    time_t t = time(nullptr);
    tm* localTm = localtime(&t);
    char buf[16];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localTm);
    return std::string(buf);
}

// ---------------------------------------------------------------------
// core algorithms
// ---------------------------------------------------------------------

// Binary search on the ID-sorted patients array.
int Clinic::binarySearchById(int id, int& comparisons) const {
    comparisons = 0;
    int lo = 0, hi = patientCount - 1;
    while (lo <= hi) {
        comparisons++;
        int mid = lo + (hi - lo) / 2;
        int midId = patients[mid]->getId();
        if (midId == id) {
            return mid;
        } else if (midId < id) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

Doctor* Clinic::findDoctorById(int id) const {
    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i]->getId() == id) {
            return doctors[i];
        }
    }
    return nullptr;
}

// Selection sort (written by hand, no std::sort) over a temporary copy
// of the patient pointers, so the real array stays sorted by ID for
// binary search. Counts every comparison made.
void Clinic::sortPatients() {
    if (patientCount == 0) {
        std::cout << "\nNo patients to sort.\n";
        return;
    }

    std::cout << "Sort by (1) name  (2) age: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (choice != 1 && choice != 2) {
        std::cout << "[X] Invalid choice.\n";
        return;
    }

    Patient* temp[MAX_PATIENTS];
    for (int i = 0; i < patientCount; i++) {
        temp[i] = patients[i];
    }

    int comparisons = 0;
    for (int i = 0; i < patientCount - 1; i++) {
        int bestIndex = i;
        for (int j = i + 1; j < patientCount; j++) {
            comparisons++;
            bool jIsBetter = (choice == 1)
                ? (temp[j]->getName() < temp[bestIndex]->getName())
                : (temp[j]->getAge() < temp[bestIndex]->getAge());
            if (jIsBetter) {
                bestIndex = j;
            }
        }
        if (bestIndex != i) {
            std::swap(temp[i], temp[bestIndex]);
        }
    }

    std::cout << "\nID    NAME             AGE\n";
    for (int i = 0; i < patientCount; i++) {
        std::cout << std::left << std::setw(6) << temp[i]->getId()
                  << std::setw(17) << temp[i]->getName()
                  << temp[i]->getAge() << "\n";
    }
    std::cout << "\nSorted in " << comparisons << " comparisons        [selection sort]\n";
}

// ---------------------------------------------------------------------
// patient operations
// ---------------------------------------------------------------------

void Clinic::addPatient() {
    if (patientCount >= MAX_PATIENTS) {
        std::cout << "[X] Patient list is full.\n";
        return;
    }

    int id = readInt("ID    : ");
    int comparisons;
    if (binarySearchById(id, comparisons) != -1) {
        std::cout << "[X] A patient with ID " << id << " already exists.\n";
        return;
    }

    std::string name = readLine("Name  : ");
    int age = readInt("Age   : ");
    std::string phone = readLine("Phone : ");

    Patient* p = new Patient(id, name, age, phone);

    // insert while keeping the array sorted by ID
    int pos = patientCount;
    while (pos > 0 && patients[pos - 1]->getId() > id) {
        patients[pos] = patients[pos - 1];
        pos--;
    }
    patients[pos] = p;
    patientCount++;

    std::cout << "\n[OK] Patient " << id << " added.\n";
}

void Clinic::viewAllPatients() const {
    std::cout << "\nID    NAME             AGE   PHONE\n";
    std::cout << "---   ---------------  ---   -----------\n";
    for (int i = 0; i < patientCount; i++) {
        std::cout << *patients[i] << "\n";
    }
    std::cout << "\n" << patientCount << " patients.\n";
}

void Clinic::searchPatientById() const {
    int id = readInt("Patient ID: ");
    int comparisons;
    int idx = binarySearchById(id, comparisons);
    if (idx == -1) {
        std::cout << "\n[X] No patient with ID " << id << ".\n";
        return;
    }
    std::cout << "\nFound in " << comparisons << " comparisons   [binary search]\n";
    patients[idx]->displayInfo();
}

void Clinic::deletePatient() {
    int id = readInt("Patient ID: ");
    int comparisons;
    int idx = binarySearchById(id, comparisons);
    if (idx == -1) {
        std::cout << "\n[X] No patient with ID " << id << ".\n";
        return;
    }
    delete patients[idx];
    for (int i = idx; i < patientCount - 1; i++) {
        patients[i] = patients[i + 1];
    }
    patientCount--;
    std::cout << "[OK] Patient " << id << " deleted.\n";
}

void Clinic::viewPatientHistory() const {
    int id = readInt("Patient ID: ");
    int comparisons;
    int idx = binarySearchById(id, comparisons);
    if (idx == -1) {
        std::cout << "\n[X] No patient with ID " << id << ".\n";
        return;
    }
    patients[idx]->printHistory();
}

// ---------------------------------------------------------------------
// doctor operations
// ---------------------------------------------------------------------

void Clinic::addDoctor() {
    if (doctorCount >= MAX_DOCTORS) {
        std::cout << "[X] Doctor list is full.\n";
        return;
    }
    int id = readInt("ID             : ");
    if (findDoctorById(id) != nullptr) {
        std::cout << "[X] A doctor with ID " << id << " already exists.\n";
        return;
    }
    std::string name = readLine("Name           : ");
    std::string spec = readLine("Specialization : ");
    double fee = -1;
    while (true) {
        std::cout << "Fee            : ";
        std::cin >> fee;
        if (std::cin.eof()) {
            std::cout << "\n[X] No more input. Exiting.\n";
            std::exit(0);
        }
        if (std::cin.fail() || fee < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[X] Please enter a valid fee.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        break;
    }

    doctors[doctorCount++] = new Doctor(id, name, spec, fee);
    std::cout << "\n[OK] Doctor " << id << " added.\n";
}

void Clinic::viewAllDoctors() const {
    if (doctorCount == 0) {
        std::cout << "\nNo doctors registered.\n";
        return;
    }
    std::cout << "\n";
    for (int i = 0; i < doctorCount; i++) {
        doctors[i]->displayInfo();
        if (i != doctorCount - 1) std::cout << "-----\n";
    }
    std::cout << "\n" << doctorCount << " doctors.\n";
}

// ---------------------------------------------------------------------
// waiting room operations
// ---------------------------------------------------------------------

void Clinic::addToWaitingRoom() {
    int id = readInt("Patient ID : ");
    int comparisons;
    int idx = binarySearchById(id, comparisons);
    if (idx == -1) {
        std::cout << "\n[X] No patient with ID " << id << ".\n";
        return;
    }
    if (waitingRoom.isFull()) {
        std::cout << "[X] Waiting room is full.\n";
        return;
    }

    std::cout << "Case type  : (1) Emergency  (2) Normal\n";
    int choice = readInt("Choose     : ");
    while (choice != 1 && choice != 2) {
        std::cout << "[X] Please choose 1 or 2.\n";
        choice = readInt("Choose     : ");
    }
    bool isEmergency = (choice == 1);
    std::string arrival = currentTimeString();
    waitingRoom.addPatient(id, isEmergency, arrival);

    std::cout << "\n[OK] " << patients[idx]->getName() << " added as "
              << (isEmergency ? "EMERGENCY" : "NORMAL") << " at " << arrival << ".\n";
}

void Clinic::viewWaitingRoom() const {
    waitingRoom.view();
    if (!waitingRoom.isEmpty()) {
        std::cout << "\n";
    }
}

void Clinic::callNextPatient() {
    if (waitingRoom.isEmpty()) {
        std::cout << "\n[X] No patients waiting.\n";
        return;
    }

    WaitingRoom::Entry entry = waitingRoom.callNext();
    int comparisons;
    int idx = binarySearchById(entry.patientId, comparisons);
    Patient* patient = (idx != -1) ? patients[idx] : nullptr;

    std::cout << "\n>>> NOW CALLING: " << (patient ? patient->getName() : "Unknown patient")
              << "  (" << (entry.isEmergency ? "EMERGENCY" : "NORMAL") << ")\n";

    if (doctorCount == 0) {
        std::cout << "[X] No doctors registered - visit cannot be recorded.\n";
        return;
    }

    int doctorId = readInt("Doctor ID  : ");
    Doctor* doc = findDoctorById(doctorId);
    while (doc == nullptr) {
        std::cout << "[X] No doctor with ID " << doctorId << ".\n";
        doctorId = readInt("Doctor ID  : ");
        doc = findDoctorById(doctorId);
    }
    std::string diagnosis = readLine("Diagnosis  : ");
    std::string date = currentDateString();
    std::string doctorLabel = "Dr. " + doc->getName();

    Visit* visit;
    std::string tag;
    if (entry.isEmergency) {
        visit = new EmergencyVisit(date, doctorLabel, diagnosis);
        tag = "EmergencyVisit::calculateFee";
    } else {
        visit = new NormalVisit(date, doctorLabel, diagnosis);
        tag = "NormalVisit::calculateFee";
    }

    double fee = visit->calculateFee(); // polymorphic call
    std::cout << "\nFee: " << std::fixed << std::setprecision(0) << fee
              << " EGP            [" << tag << "]\n";

    if (patient != nullptr) {
        patient->addVisit(visit); // patient now owns the Visit*
        std::cout << "\n[OK] Visit added to history. ";
        if (waitingRoom.isEmpty()) {
            std::cout << "Waiting room is empty.\n";
        } else {
            std::cout << waitingRoom.getCount() << " patient(s) still waiting.\n";
        }
    } else {
        delete visit;
        std::cout << "[X] Patient record not found; visit could not be saved.\n";
    }
}

// ---------------------------------------------------------------------
// appointment operations (bonus feature)
// ---------------------------------------------------------------------

// A double booking = same doctor, same date, same time already taken.
bool Clinic::isDoctorBooked(int doctorId, const std::string& date, const std::string& time) const {
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i]->getDoctorId() == doctorId &&
            appointments[i]->getDate() == date &&
            appointments[i]->getTime() == time) {
            return true;
        }
    }
    return false;
}

void Clinic::bookAppointment() {
    int patientId = readInt("Patient ID         : ");
    int comparisons;
    int pIdx = binarySearchById(patientId, comparisons);
    if (pIdx == -1) {
        std::cout << "\n[X] No patient with ID " << patientId << ".\n";
        return;
    }

    int doctorId = readInt("Doctor ID          : ");
    Doctor* doc = findDoctorById(doctorId);
    if (doc == nullptr) {
        std::cout << "\n[X] No doctor with ID " << doctorId << ".\n";
        return;
    }

    std::string date = readLine("Date (YYYY-MM-DD)  : ");
    std::string time = readLine("Time (e.g. 3:00 PM): ");

    if (isDoctorBooked(doctorId, date, time)) {
        std::cout << "\n[X] Dr. " << doc->getName() << " already has an appointment on "
                  << date << " at " << time << ". Choose another slot.\n";
        return;
    }

    if (appointmentCount >= MAX_APPOINTMENTS) {
        std::cout << "[X] Appointment list is full.\n";
        return;
    }

    appointments[appointmentCount++] = new Appointment(patientId, doctorId, date, time);
    std::cout << "\n[OK] Appointment booked: " << patients[pIdx]->getName()
              << " with Dr. " << doc->getName() << " on " << date
              << " at " << time << ".\n";
}

void Clinic::viewAllAppointments() const {
    if (appointmentCount == 0) {
        std::cout << "\nNo appointments booked.\n";
        return;
    }

    std::cout << "\nPATIENT           DOCTOR            DATE         TIME\n";
    std::cout << "---------------   ---------------   ----------   --------\n";
    for (int i = 0; i < appointmentCount; i++) {
        int comparisons;
        int pIdx = binarySearchById(appointments[i]->getPatientId(), comparisons);
        Doctor* doc = findDoctorById(appointments[i]->getDoctorId());

        std::string pname = (pIdx != -1) ? patients[pIdx]->getName() : "Unknown patient";
        std::string dname = (doc != nullptr) ? ("Dr. " + doc->getName()) : "Unknown doctor";

        std::cout << std::left << std::setw(18) << pname
                  << std::setw(18) << dname
                  << std::setw(13) << appointments[i]->getDate()
                  << appointments[i]->getTime() << "\n";
    }
    std::cout << "\n" << appointmentCount << " appointment(s).\n";
}

// ---------------------------------------------------------------------
// sub-menus
// ---------------------------------------------------------------------

void Clinic::patientsMenu() {
    int choice;
    do {
        std::cout << "\n------------- PATIENTS ------------------\n"
                   << "1. Add patient\n"
                   << "2. View all patients\n"
                   << "3. Search by ID\n"
                   << "4. Delete patient\n"
                   << "5. View patient history\n"
                   << "6. Sort patients\n"
                   << "0. Back\n"
                   << "-----------------------------------------\n";
        choice = readInt("Choose: ");
        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewAllPatients(); break;
            case 3: searchPatientById(); break;
            case 4: deletePatient(); break;
            case 5: viewPatientHistory(); break;
            case 6: sortPatients(); break;
            case 0: break;
            default: std::cout << "[X] Invalid choice.\n";
        }
    } while (choice != 0);
}

void Clinic::doctorsMenu() {
    int choice;
    do {
        std::cout << "\n------------- DOCTORS -------------------\n"
                   << "1. Add doctor\n"
                   << "2. View all doctors\n"
                   << "0. Back\n"
                   << "-----------------------------------------\n";
        choice = readInt("Choose: ");
        switch (choice) {
            case 1: addDoctor(); break;
            case 2: viewAllDoctors(); break;
            case 0: break;
            default: std::cout << "[X] Invalid choice.\n";
        }
    } while (choice != 0);
}

void Clinic::waitingRoomMenu() {
    int choice;
    do {
        std::cout << "\n---------- WAITING ROOM -----------------\n"
                   << "1. Add to waiting room\n"
                   << "2. View who is waiting\n"
                   << "3. Call next patient\n"
                   << "0. Back\n"
                   << "-----------------------------------------\n";
        choice = readInt("Choose: ");
        switch (choice) {
            case 1: addToWaitingRoom(); break;
            case 2: viewWaitingRoom(); break;
            case 3: callNextPatient(); break;
            case 0: break;
            default: std::cout << "[X] Invalid choice.\n";
        }
    } while (choice != 0);
}

void Clinic::appointmentsMenu() {
    int choice;
    do {
        std::cout << "\n---------- APPOINTMENTS ------------------\n"
                   << "1. Book appointment\n"
                   << "2. View all appointments\n"
                   << "0. Back\n"
                   << "-----------------------------------------\n";
        choice = readInt("Choose: ");
        switch (choice) {
            case 1: bookAppointment(); break;
            case 2: viewAllAppointments(); break;
            case 0: break;
            default: std::cout << "[X] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ---------------------------------------------------------------------
// main loop
// ---------------------------------------------------------------------

void Clinic::run() {
    int choice;
    do {
        std::cout << "\n=========================================\n"
                   << "      CLINIC MANAGEMENT SYSTEM\n"
                   << "=========================================\n"
                   << "1. Patients\n"
                   << "2. Doctors\n"
                   << "3. Waiting Room\n"
                   << "4. Appointments\n"
                   << "0. Exit\n"
                   << "-----------------------------------------\n";
        choice = readInt("Choose: ");
        switch (choice) {
            case 1: patientsMenu(); break;
            case 2: doctorsMenu(); break;
            case 3: waitingRoomMenu(); break;
            case 4: appointmentsMenu(); break;
            case 0: std::cout << "\nGoodbye.\n"; break;
            default: std::cout << "[X] Invalid choice.\n";
        }
    } while (choice != 0);
}
