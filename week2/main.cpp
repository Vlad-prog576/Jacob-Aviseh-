#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Student {
    string indexNumber;
    string fullName;
    string programme;
};

struct AttendanceSession {
    string courseCode;
    string date;
    string startTime;
    int durationMinutes;
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

int readInt(const string& prompt, int minValue, int maxValue) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        try {
            int value = stoi(line);
            if (value >= minValue && value <= maxValue) {
                return value;
            }
        } catch (...) {
        }
        cout << "Enter a valid number between " << minValue << " and " << maxValue << ".\n";
    }
}

void registerStudent(vector<Student>& students) {
    Student s;
    s.indexNumber = readNonEmpty("Index Number: ");
    s.fullName = readNonEmpty("Full Name: ");
    s.programme = readNonEmpty("Programme: ");
    students.push_back(s);
    cout << "Student registered.\n";
}

void viewStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students yet.\n";
        return;
    }
    cout << left << setw(15) << "Index" << setw(30) << "Name" << "Programme\n";
    cout << string(60, '-') << "\n";
    for (const Student& s : students) {
        cout << left << setw(15) << s.indexNumber << setw(30) << s.fullName << s.programme << "\n";
    }
}

void createSession(vector<AttendanceSession>& sessions) {
    AttendanceSession session;
    session.courseCode = readNonEmpty("Course Code: ");
    session.date = readNonEmpty("Date (YYYY-MM-DD): ");
    session.startTime = readNonEmpty("Start Time (HH:MM): ");
    session.durationMinutes = readInt("Duration (minutes): ", 1, 600);
    sessions.push_back(session);
    cout << "Session created.\n";
}

void viewSessions(const vector<AttendanceSession>& sessions) {
    if (sessions.empty()) {
        cout << "No sessions yet.\n";
        return;
    }

    cout << "\n--- Sessions ---\n";
    for (size_t i = 0; i < sessions.size(); ++i) {
        cout << i + 1 << ". " << sessions[i].courseCode << " | " << sessions[i].date << " | "
             << sessions[i].startTime << " | " << sessions[i].durationMinutes << " mins\n";
    }
}

int main() {
    vector<Student> students;
    vector<AttendanceSession> sessions;

    while (true) {
        cout << "\nWEEK 2 MENU\n";
        cout << "1. Register Student\n";
        cout << "2. View Students\n";
        cout << "3. Create Lecture Session\n";
        cout << "4. View Lecture Sessions\n";
        cout << "0. Exit\n";

        int choice = readInt("Choose: ", 0, 4);
        if (choice == 1) registerStudent(students);
        if (choice == 2) viewStudents(students);
        if (choice == 3) createSession(sessions);
        if (choice == 4) viewSessions(sessions);
        if (choice == 0) break;
    }

    cout << "Goodbye.\n";
    return 0;
}
