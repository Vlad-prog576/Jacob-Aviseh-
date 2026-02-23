#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Student {
    string indexNumber;
    string fullName;
};

struct AttendanceRecord {
    string studentIndex;
    string status;
};

struct AttendanceSession {
    string courseCode;
    string date;
    string startTime;
    int durationMinutes;
    vector<AttendanceRecord> records;
};

string readNonEmpty(const string& prompt) {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        if (!input.empty()) return input;
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
            if (value >= minValue && value <= maxValue) return value;
        } catch (...) {
        }
        cout << "Invalid number. Enter between " << minValue << " and " << maxValue << ".\n";
    }
}

bool validStatus(const string& s) {
    return s == "Present" || s == "Absent" || s == "Late";
}

void registerStudent(vector<Student>& students) {
    Student s;
    s.indexNumber = readNonEmpty("Index Number: ");
    s.fullName = readNonEmpty("Full Name: ");
    students.push_back(s);
    cout << "Student registered.\n";
}

void viewStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students yet.\n";
        return;
    }

    cout << left << setw(15) << "Index" << "Name\n";
    cout << string(40, '-') << "\n";
    for (const Student& s : students) {
        cout << left << setw(15) << s.indexNumber << s.fullName << "\n";
    }
}

void createSession(vector<AttendanceSession>& sessions) {
    AttendanceSession s;
    s.courseCode = readNonEmpty("Course Code: ");
    s.date = readNonEmpty("Date (YYYY-MM-DD): ");
    s.startTime = readNonEmpty("Start Time (HH:MM): ");
    s.durationMinutes = readInt("Duration (minutes): ", 1, 600);
    sessions.push_back(s);
    cout << "Session created.\n";
}

void viewSessions(const vector<AttendanceSession>& sessions) {
    if (sessions.empty()) {
        cout << "No sessions yet.\n";
        return;
    }

    for (size_t i = 0; i < sessions.size(); ++i) {
        cout << i + 1 << ". " << sessions[i].courseCode << " | " << sessions[i].date << " | "
             << sessions[i].startTime << " | " << sessions[i].durationMinutes << " mins\n";
    }
}

int chooseSession(const vector<AttendanceSession>& sessions) {
    if (sessions.empty()) {
        cout << "No sessions available.\n";
        return -1;
    }
    viewSessions(sessions);
    return readInt("Select session: ", 1, static_cast<int>(sessions.size())) - 1;
}

int findRecordIndex(const vector<AttendanceRecord>& records, const string& indexNumber) {
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].studentIndex == indexNumber) return static_cast<int>(i);
    }
    return -1;
}

void markAttendance(vector<AttendanceSession>& sessions, const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students available. Register students first.\n";
        return;
    }

    int idx = chooseSession(sessions);
    if (idx < 0) return;

    for (const Student& st : students) {
        cout << st.indexNumber << " - " << st.fullName << " status (Present/Absent/Late): ";
        string status;
        getline(cin, status);
        while (!validStatus(status)) {
            cout << "Enter only Present, Absent, or Late: ";
            getline(cin, status);
        }

        int recordIndex = findRecordIndex(sessions[idx].records, st.indexNumber);
        if (recordIndex == -1) {
            sessions[idx].records.push_back({st.indexNumber, status});
        } else {
            sessions[idx].records[recordIndex].status = status;
        }
    }

    cout << "Attendance saved for session.\n";
}

void updateAttendance(vector<AttendanceSession>& sessions) {
    int idx = chooseSession(sessions);
    if (idx < 0) return;

    string indexNumber = readNonEmpty("Enter student index to update: ");
    int recordIndex = findRecordIndex(sessions[idx].records, indexNumber);
    if (recordIndex == -1) {
        cout << "No attendance record for that student in this session.\n";
        return;
    }

    cout << "Current status: " << sessions[idx].records[recordIndex].status << "\n";
    string status = readNonEmpty("New status (Present/Absent/Late): ");
    while (!validStatus(status)) {
        status = readNonEmpty("Enter only Present, Absent, or Late: ");
    }

    sessions[idx].records[recordIndex].status = status;
    cout << "Attendance updated.\n";
}

void attendanceList(const vector<AttendanceSession>& sessions, const vector<Student>& students) {
    int idx = chooseSession(sessions);
    if (idx < 0) return;

    if (sessions[idx].records.empty()) {
        cout << "No attendance marked for this session.\n";
        return;
    }

    cout << "Session: " << sessions[idx].courseCode << " " << sessions[idx].date << "\n";
    cout << left << setw(15) << "Index" << setw(30) << "Name" << "Status\n";
    cout << string(60, '-') << "\n";

    for (const auto& r : sessions[idx].records) {
        string name = "Unknown";
        for (const auto& s : students) {
            if (s.indexNumber == r.studentIndex) {
                name = s.fullName;
                break;
            }
        }
        cout << left << setw(15) << r.studentIndex << setw(30) << name << r.status << "\n";
    }
}

void summary(const vector<AttendanceSession>& sessions) {
    int idx = chooseSession(sessions);
    if (idx < 0) return;

    int present = 0, absent = 0, late = 0;
    for (const auto& r : sessions[idx].records) {
        if (r.status == "Present") present++;
        if (r.status == "Absent") absent++;
        if (r.status == "Late") late++;
    }
    cout << "Present: " << present << "\nAbsent: " << absent << "\nLate: " << late << "\n";
}

int main() {
    vector<Student> students;
    vector<AttendanceSession> sessions;

    while (true) {
        cout << "\nWEEK 3 MENU\n";
        cout << "1. Register Student\n";
        cout << "2. View Students\n";
        cout << "3. Create Session\n";
        cout << "4. View Sessions\n";
        cout << "5. Mark Attendance\n";
        cout << "6. Update Attendance\n";
        cout << "7. Attendance List\n";
        cout << "8. Attendance Summary\n";
        cout << "0. Exit\n";

        int choice = readInt("Choose: ", 0, 8);
        if (choice == 1) registerStudent(students);
        if (choice == 2) viewStudents(students);
        if (choice == 3) createSession(sessions);
        if (choice == 4) viewSessions(sessions);
        if (choice == 5) markAttendance(sessions, students);
        if (choice == 6) updateAttendance(sessions);
        if (choice == 7) attendanceList(sessions, students);
        if (choice == 8) summary(sessions);
        if (choice == 0) break;
    }

    cout << "Goodbye.\n";
    return 0;
}
