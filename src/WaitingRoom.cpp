#include "WaitingRoom.h"
#include <iostream>

WaitingRoom::WaitingRoom() : count(0), nextArrivalOrder(0) {
    for (int i = 0; i < CAPACITY; i++) {
        entries[i].used = false;
    }
}

bool WaitingRoom::isEmpty() const {
    return count == 0;
}

bool WaitingRoom::isFull() const {
    return count >= CAPACITY;
}

int WaitingRoom::getCount() const {
    return count;
}

void WaitingRoom::addPatient(int patientId, bool isEmergency, const std::string& arrivalTime) {
    if (isFull()) return;

    // find a free slot
    for (int i = 0; i < CAPACITY; i++) {
        if (!entries[i].used) {
            entries[i].patientId = patientId;
            entries[i].isEmergency = isEmergency;
            entries[i].arrivalTime = arrivalTime;
            entries[i].arrivalOrder = nextArrivalOrder++;
            entries[i].used = true;
            count++;
            return;
        }
    }
}

// Priority rule: Emergency beats Normal. Same category -> smaller
// arrivalOrder (arrived earlier) wins. This is the heart of the
// "priority queue, not first-come-first-served" requirement.
int WaitingRoom::findHighestPriorityIndex() const {
    int bestIndex = -1;
    for (int i = 0; i < CAPACITY; i++) {
        if (!entries[i].used) continue;
        if (bestIndex == -1) {
            bestIndex = i;
            continue;
        }
        bool currentIsBetter;
        if (entries[i].isEmergency != entries[bestIndex].isEmergency) {
            currentIsBetter = entries[i].isEmergency; // emergency wins
        } else {
            currentIsBetter = entries[i].arrivalOrder < entries[bestIndex].arrivalOrder;
        }
        if (currentIsBetter) {
            bestIndex = i;
        }
    }
    return bestIndex;
}

WaitingRoom::Entry WaitingRoom::callNext() {
    int idx = findHighestPriorityIndex();
    Entry result = entries[idx];
    entries[idx].used = false;
    count--;
    return result;
}

void WaitingRoom::view() const {
    if (isEmpty()) {
        std::cout << "\nWaiting room is empty.\n";
        return;
    }

    // Print entries in priority order without mutating the real queue:
    // walk a temporary "already shown" list and repeatedly pick the
    // best remaining entry.
    bool shown[CAPACITY] = {false};

    std::cout << "\n# " << "PATIENT ID   " << "CASE         " << "ARRIVED\n";
    int rank = 1;
    for (int shownCount = 0; shownCount < count; shownCount++) {
        int bestIndex = -1;
        for (int i = 0; i < CAPACITY; i++) {
            if (!entries[i].used || shown[i]) continue;
            if (bestIndex == -1) {
                bestIndex = i;
                continue;
            }
            bool currentIsBetter;
            if (entries[i].isEmergency != entries[bestIndex].isEmergency) {
                currentIsBetter = entries[i].isEmergency;
            } else {
                currentIsBetter = entries[i].arrivalOrder < entries[bestIndex].arrivalOrder;
            }
            if (currentIsBetter) {
                bestIndex = i;
            }
        }
        shown[bestIndex] = true;
        std::cout << rank << "  " << entries[bestIndex].patientId
                  << "          " << (entries[bestIndex].isEmergency ? "EMERGENCY" : "NORMAL")
                  << "      " << entries[bestIndex].arrivalTime << "\n";
        rank++;
    }
}
