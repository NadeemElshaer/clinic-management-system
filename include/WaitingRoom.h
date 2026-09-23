#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include <string>

// A priority queue built from scratch on top of a plain array (no
// std::queue / std::vector). Emergency cases always come out before
// Normal cases; within the same case type, whoever arrived first is
// served first (FIFO tie-break via arrivalOrder).
class WaitingRoom {
public:
    struct Entry {
        int patientId;
        bool isEmergency;
        std::string arrivalTime; // display string, e.g. "3:10 PM"
        int arrivalOrder;        // increasing counter -> arrival sequence
        bool used;                // marks a live slot in the array
    };

private:
    static const int CAPACITY = 200;
    Entry entries[CAPACITY];
    int count;
    int nextArrivalOrder;

    // Finds the array index of the highest-priority waiting entry.
    // Returns -1 if the room is empty.
    int findHighestPriorityIndex() const;

public:
    WaitingRoom();

    bool isEmpty() const;
    bool isFull() const;
    int getCount() const;

    void addPatient(int patientId, bool isEmergency, const std::string& arrivalTime);

    // Removes and returns the highest-priority entry.
    // Caller must check isEmpty() first.
    Entry callNext();

    void view() const; // prints everyone waiting, in priority order
};

#endif
