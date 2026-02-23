#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Student {
   public:
    string indexNumber;
    string fullName;
    string programme;

    Student(const string& index, const string& name, const string& prog)
        : indexNumber(index), fullName(name), programme(prog) {}
};

string readNonEmpty(const string& prompt) {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        if (!input.empty()) {
            return input;
        }
        cout << "Input cannot be empty.\n";
    }
}

void registerStudent(vector<Student>& students) {
    cout << "\n--- Register Student ---\n";
    string index = readNonEmpty("Index Number: ");

    for (const Student& s : students) {
        if (s.indexNumber == index) {
            cout << "Index number already exists.\n";
            return;
        }
    }

    string name = readNonEmpty("Full Name: ");
    string programme = readNonEmpty("Programme: ");
    students.emplace_back(index, name, programme);

    cout << "Student added successfully.\n";
}

void viewStudents(const vector<Student>& students) {
    cout << "\n--- Registered Students ---\n";
    if (students.empty()) {
        cout << "No students registered yet.\n";
        return;
    }

    cout << left << setw(15) << "Index" << setw(30) << "Name" << "Programme\n";
    cout << string(60, '-') << "\n";
    for (const Student& s : students) {
        cout << left << setw(15) << s.indexNumber << setw(30) << s.fullName << s.programme << "\n";
    }
}

int readChoice() {
    while (true) {
        cout << "\n1. Register Student\n2. View Students\n0. Exit\nChoose: ";
        string input;
        getline(cin, input);
        if (input == "0" || input == "1" || input == "2") {
            return stoi(input);
        }
        cout << "Invalid choice.\n";
    }
}

int main() {
    vector<Student> students;

    cout << "WEEK 1 - Digital Attendance (Student Basics)\n";
    while (true) {
        int choice = readChoice();
        if (choice == 1) {
            registerStudent(students);
        } else if (choice == 2) {
            viewStudents(students);
        } else {
            cout << "Goodbye.\n";
            break;
        }
    }
    return 0;
}
