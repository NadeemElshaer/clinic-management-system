#include "VisitHistory.h"
#include <iostream>
#include <iomanip>

VisitHistory::VisitHistory() : head(nullptr), tail(nullptr), count(0) {}

VisitHistory::~VisitHistory() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current->visit; // free the Visit object the node owns
        delete current;        // free the node itself
        current = next;
    }
    head = nullptr;
    tail = nullptr;
}

void VisitHistory::addVisit(Visit* visit) {
    Node* newNode = new Node(visit);
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    count++;
}

void VisitHistory::printHistory(const std::string& patientName) const {
    std::cout << "\nVISIT HISTORY - " << patientName << "            [linked list]\n";
    std::cout << "--------------------------------------------------\n";

    if (isEmpty()) {
        std::cout << "No visits recorded yet.\n";
        return;
    }

    Node* current = head;
    while (current != nullptr) {
        Visit* v = current->visit;
        std::cout << v->getDate() << "  " << v->getDoctorName()
                  << "  " << v->getDiagnosis()
                  << "        " << std::fixed << std::setprecision(0)
                  << v->calculateFee() << " EGP\n";
        current = current->next;
    }
    std::cout << "--------------------------------------------------\n";
    std::cout << "Total paid: " << std::fixed << std::setprecision(0)
              << getTotalPaid() << " EGP               [recursive sum]\n";
}

double VisitHistory::sumFeesRecursive(Node* node) const {
    if (node == nullptr) {       // base case
        return 0.0;
    }
    return node->visit->calculateFee() + sumFeesRecursive(node->next); // recursive case
}

double VisitHistory::getTotalPaid() const {
    return sumFeesRecursive(head);
}

int VisitHistory::getCount() const {
    return count;
}

bool VisitHistory::isEmpty() const {
    return head == nullptr;
}
