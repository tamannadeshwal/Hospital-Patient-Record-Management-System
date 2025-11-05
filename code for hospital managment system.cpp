#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <cmath>
using namespace std;

// -- Linked List for Patient Records --
struct PatientNode {
    int id;
    string name;
    string date;
    string treatment;
    PatientNode* next;
    PatientNode(int i, string n, string d, string t) :
        id(i), name(n), date(d), treatment(t), next(nullptr) {}
};

class PatientList {
public:
    PatientNode* head;
    PatientList() { head = nullptr; }
    
    void addPatient(int id, string name, string date, string treatment) {
        // Add at the beginning for simplicity
        PatientNode* newPatient = new PatientNode(id, name, date, treatment);
        newPatient->next = head;
        head = newPatient;
        cout << "Patient added successfully.\n";
    }

    bool removePatient(int id) {
        PatientNode* curr = head, *prev = nullptr;
        while (curr && curr->id != id) {
            prev = curr; curr = curr->next;
        }
        if (!curr) return false; // not found
        if (!prev) head = curr->next; // at head
        else prev->next = curr->next;
        delete curr;
        cout << "Patient removed.\n";
        return true;
    }

    bool getPatient(int id, string& name, string& date, string& treatment) {
        PatientNode* curr = head;
        while (curr) {
            if (curr->id == id) {
                name = curr->name; date = curr->date; treatment = curr->treatment;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    void displayAll() {
        PatientNode* curr = head;
        while (curr) {
            cout << "ID: " << curr->id 
                 << ", Name: " << curr->name 
                 << ", Date: " << curr->date 
                 << ", Treatment: " << curr->treatment << endl;
            curr = curr->next;
        }
    }
};

// -- Stack for Undo Admission/Deletion --
struct UndoAction {
    string type; // "add" or "delete"
    int id;
    string name, date, treatment;
};

class UndoStack {
    stack<UndoAction> actions;
public:
    void addAction(string type, int id, string name, string date, string treatment) {
        actions.push({type, id, name, date, treatment});
    }

    void undo(PatientList& plist) {
        if (actions.empty()) {
            cout << "Nothing to undo.\n";
            return;
        }
        UndoAction action = actions.top(); actions.pop();
        if (action.type == "add") {
            plist.removePatient(action.id);
            cout << "Undo add.\n";
        } else if (action.type == "delete") {
            plist.addPatient(action.id, action.name, action.date, action.treatment);
            cout << "Undo delete.\n";
        }
    }
};

// -- Emergency Queue (Simple Circular Queue and Priority Concept) --
struct EmergencyPatient {
    int id;
    string name;
    int severity; // bigger number = higher severity
    EmergencyPatient(int i, string n, int s): id(i), name(n), severity(s) {}
};
struct CompareSeverity {
    bool operator()(EmergencyPatient const& a, EmergencyPatient const& b) {
        return a.severity < b.severity; // max heap
    }
};
queue<EmergencyPatient> circQueue;
priority_queue<EmergencyPatient, vector<EmergencyPatient>, CompareSeverity> priorityQueue;

void admitEmergency(int id, string name, int severity) {
    EmergencyPatient ep(id, name, severity);
    circQueue.push(ep); // ordinary order
    priorityQueue.push(ep); // severity order
    cout << "Emergency patient added.\n";
}

void serveEmergency() {
    if (circQueue.empty()) cout << "No emergency patient (circular queue).\n";
    else {
        EmergencyPatient ep = circQueue.front();
        circQueue.pop();
        cout << "Serving (circular queue): ID " << ep.id << " Name: " << ep.name << endl;
    }
    if(priorityQueue.empty()) cout << "No emergency patient (priority).\n";
    else {
        EmergencyPatient ep = priorityQueue.top();
        priorityQueue.pop();
        cout << "Serving (priority, high severity): ID " << ep.id << " Name: " << ep.name << endl;
    }
}

// -- Polynomial for Billing (as sum of terms c*x^e) --
struct Term {
    int coeff;
    int exp;
};
class Polynomial {
public:
    vector<Term> terms;
    void addTerm(int c, int e) {
        terms.push_back({c,e});
        cout << "Term added to bill: " << c << "x^" << e << endl;
    }
    int power(int base, int exp) {
        int result = 1;
        for (int i = 0; i < exp; i++) result *= base;
        return result;
    }
    int calculate(int x) {
        int sum = 0;
        for (auto t : terms)
            sum += t.coeff * power(x, t.exp);
        return sum;
    }
    void display() {
        for (auto t : terms)
            cout << t.coeff << "x^" << t.exp << " + ";
        cout << "\b\b  \n";
    }
};

// -------------------- MAIN PROGRAM STARTS HERE -------------------
int main() {
    PatientList patients;
    UndoStack undo;
    Polynomial billing;
    int choice;
    
    while(true) {
        cout << "\nMenu:\n";
        cout << "1. Add Patient\n";
        cout << "2. Remove Patient\n";
        cout << "3. Display Patients\n";
        cout << "4. Undo Last Action\n";
        cout << "5. Add Emergency Case\n";
        cout << "6. Serve Emergency Case\n";
        cout << "7. Add Billing Term\n";
        cout << "8. Calculate Bill\n";
        cout << "0. Exit\n";
        cin >> choice;
        
        if (choice == 0) break;

        if (choice == 1) {
            int id; string name, date, treatment;
            cout << "Patient ID: "; cin >> id;
            cout << "Name: "; cin >> name;
            cout << "Admission Date: "; cin >> date;
            cout << "Treatment: "; cin >> treatment;
            patients.addPatient(id, name, date, treatment);
            undo.addAction("add", id, name, date, treatment);
        }
        else if (choice == 2) {
            int id; string name, date, treatment;
            cout << "Patient ID to remove: "; cin >> id;
            // Fetch for undo before removing
            if (patients.getPatient(id, name, date, treatment)) {
                patients.removePatient(id);
                undo.addAction("delete", id, name, date, treatment);
            } else cout << "Patient not found.\n";
        }
        else if (choice == 3) patients.displayAll();
        else if (choice == 4) undo.undo(patients);
        else if (choice == 5) {
            int id, sev; string name;
            cout << "Emergency Patient ID: "; cin >> id;
            cout << "Name: "; cin >> name;
            cout << "Severity (1-10): "; cin >> sev;
            admitEmergency(id, name, sev);
        }
        else if (choice == 6) serveEmergency();
        else if (choice == 7) {
            int c, e;
            cout << "Enter coefficient: "; cin >> c;
            cout << "Enter exponent: "; cin >> e;
            billing.addTerm(c, e);
        }
        else if (choice == 8) {
            int x;
            billing.display();
            cout << "Enter value of x for bill calculation: "; cin >> x;
            cout << "Total Bill: " << billing.calculate(x) << endl;
        }
        else cout << "Invalid choice.\n";
    }
    return 0;
}