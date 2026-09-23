#ifndef VISITHISTORY_H
#define VISITHISTORY_H

#include <string>
#include "Visit.h"

// A singly linked list built from scratch (no std::list) that stores the
// full visit history of one patient, in the order visits happened.
class VisitHistory {
private:
    struct Node {
        Visit* visit;
        Node* next;
        Node(Visit* v) : visit(v), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

    // Recursive helper: walks the list node by node and sums the fees.
    // Base case: node == nullptr -> returns 0.
    double sumFeesRecursive(Node* node) const;

public:
    VisitHistory();
    ~VisitHistory(); // frees every node (and the Visit it owns)

    // No copying: this class owns raw pointers (Visit*), so a shallow copy
    // would double-free. Disable copy to keep memory management safe.
    VisitHistory(const VisitHistory&) = delete;
    VisitHistory& operator=(const VisitHistory&) = delete;

    void addVisit(Visit* visit);
    void printHistory(const std::string& patientName) const;
    double getTotalPaid() const; // uses the recursive helper
    int getCount() const;
    bool isEmpty() const;
};

#endif
