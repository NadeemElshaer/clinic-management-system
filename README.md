Clinic Management System

A console-based clinic management system written in C++. It stores patients and doctors, runs a waiting room that is not first-come-first-served (an emergency case always jumps ahead of a routine one, and only arrival order breaks ties within the same priority), keeps a full visit history — including fees, which are computed polymorphically — for every patient, and lets staff book dated/timed appointments while rejecting double bookings for the same doctor (bonus feature).

Team members and who did what

Fill this in with your actual team before submitting.

Name	Contribution
Member 1	Person / Patient / Doctor classes, encapsulation & getters/setters
Member 2	Visit / EmergencyVisit / NormalVisit (polymorphism), VisitHistory linked list
Member 3	WaitingRoom priority queue, Clinic menu wiring
Member 4	Binary search, sorting, README, UML diagram, testing
How to compile and run

From the project root:

bash
g++ -std=c++17 -Wall -Iinclude src/*.cpp -o clinic
./clinic

On Windows (MinGW), the same command works from a g++ shell. There are no external dependencies — only the standard library.

What the program does

Run the program and use the numbered menu to navigate: Patients (add, view, search by ID, delete, view visit history, sort), Doctors (add, view), Waiting Room (add a patient as Emergency or Normal, view who is waiting in priority order, and call the next patient — which records a new visit with a fee computed by that visit type's own calculateFee()), and Appointments (book a patient with a doctor for a specific date/time, and view every booking). Booking checks every existing appointment for that doctor and rejects the new one if the same date+time slot is already taken.

Data structures used, and why
Structure	Where	Why
Priority queue (built from scratch on a fixed array, WaitingRoom)	The waiting room	A plain FIFO queue can't express "emergency jumps the line." Each addPatient call tags an entry with a case type and an arrival-order counter; callNext() scans for the entry that is Emergency (or, failing that, has the smallest arrival order) and removes it. This gives the exact behavior the spec asks for without needing std::priority_queue.
Singly linked list (built from scratch, VisitHistory)	Each patient's visit history	Visit counts per patient are small and unbounded in principle, and history is always read start-to-end (never randomly indexed), so a linked list avoids resizing/copy costs a dynamic array would pay and makes appending a visit O(1).
Sorted array + binary search	Looking up a patient by ID	The patients array is kept sorted by ID on every insert/delete, which turns "find by ID" from O(n) into O(log n) — worthwhile since search/delete/waiting-room lookups all go through it.
Selection sort	Sorting patients by name or age on demand	Written by hand (no std::sort) as required; O(n²) is acceptable since this only runs when the user explicitly asks to view a sorted list, not on every operation.
Array of Appointment*	Bonus: appointment booking	Bookings are looked up by (doctor, date, time) to reject a double booking, and by list-everything for viewing — a plain array with a linear scan is simple and fast enough at the scale this project runs at.
Big O table
Operation	Structure	Time
Add patient (keeps array sorted)	Sorted array	O(n) worst case (shifting to insert)
Search patient by ID	Sorted array	O(log n)
Delete patient	Sorted array	O(n) (shifting after removal)
Sort patients by name/age	Array (selection sort)	O(n²)
Add visit to history	Linked list	O(1)
Recursive total paid	Linked list	O(n) time, O(n) call-stack space
Add to waiting room	Array-backed priority queue	O(1) — scans a fixed-size array of at most CAPACITY (200) slots for a free one; bounded by a compile-time constant, not by how many patients are currently waiting
Call next patient (highest priority)	Array-backed priority queue	O(1) — findHighestPriorityIndex() always scans the full fixed CAPACITY array (200 slots) regardless of how many are actually occupied, so its cost never grows with the waiting room's current size
View waiting room in order	Array-backed priority queue	O(n) — the outer loop runs count times (n = patients currently waiting), and each inner "find best remaining" scan is the same constant-bounded CAPACITY scan described above
Book appointment (double-booking check)	Array	O(n) (scans existing appointments for a clash)
View all appointments	Array	O(n)
OOP requirements — where they live
Abstract classes: Person (pure virtual displayInfo()), Visit (pure virtual calculateFee() and getCaseType()).
Inheritance: Patient and Doctor derive from Person; EmergencyVisit and NormalVisit derive from Visit.
Polymorphism: Visit::calculateFee() is called through a Visit* in Clinic::callNextPatient() — the actual fee (400 EGP emergency, 200 EGP normal) depends entirely on which concrete class was constructed, not on any if/switch in the caller.
Encapsulation: every data member across every class is private or protected; all access goes through getters/setters.
Destructors: VisitHistory::~VisitHistory() frees every linked-list node and the Visit* it owns; Clinic::~Clinic() frees every Patient* and Doctor* it allocated.
Operator overload: operator<<(ostream&, const Patient&) prints one formatted patient row.
Recursion: VisitHistory::sumFeesRecursive() walks the list node by node to total what a patient has paid (base case: nullptr → 0).
Screenshots

Add 2–3 screenshots here (as images in docs/screenshots/) of your program running: the main menu, adding/viewing patients, and the waiting room priority behavior are good ones to show.

Project structure
clinic-management-system/
├── README.md
├── docs/
│   ├── uml-class-diagram.png
│   ├── uml.dot                (Graphviz source for the diagram, optional)
│   └── screenshots/
├── include/                   ← headers (.h)
└── src/                       ← implementation (.cpp), including main.cpp