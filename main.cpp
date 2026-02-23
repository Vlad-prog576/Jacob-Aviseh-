#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Student {
    string indexNumber;
    string fullName;
    string programme;
};

struct AttendanceRecord {
    string studentIndex;
    string status; // Present, Absent, Late
};

struct AttendanceSession {
    string courseCode;
    string date;      // YYYY-MM-DD
    string startTime; // HH:MM
    int durationMinutes = 0;
    vector<AttendanceRecord> records;
};

const string STUDENTS_FILE = "students.txt";
const string SESSIONS_INDEX_FILE = "sessions_index.txt";

string trim(const string &value) {
    size_t start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

string toUpper(const string &value) {
    string result = value;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return static_cast<char>(toupper(c));
    });
    return result;
}

bool isValidDate(const string &date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }

    for (size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7) {
            continue;
        }
        if (!isdigit(static_cast<unsigned char>(date[i]))) {
            return false;
        }
    }

    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }

    return true;
}

bool isValidTime(const string &time) {
    if (time.size() != 5 || time[2] != ':') {
        return false;
    }

    for (size_t i = 0; i < time.size(); ++i) {
        if (i == 2) {
            continue;
        }
        if (!isdigit(static_cast<unsigned char>(time[i]))) {
            return false;
        }
    }

    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));

    return hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59;
}

bool isValidStatus(const string &status) {
    string upperStatus = toUpper(trim(status));
    return upperStatus == "PRESENT" || upperStatus == "ABSENT" || upperStatus == "LATE";
}

string normalizeStatus(const string &status) {
    string upperStatus = toUpper(trim(status));
    if (upperStatus == "PRESENT") {
        return "Present";
    }
    if (upperStatus == "ABSENT") {
        return "Absent";
    }
    return "Late";
}

int readInt(const string &prompt, int minValue, int maxValue) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);

        stringstream ss(trim(line));
        int value;
        char extra;

        if ((ss >> value) && !(ss >> extra) && value >= minValue && value <= maxValue) {
            return value;
        }

        cout << "Invalid input. Enter a number between " << minValue << " and " << maxValue << ".\n";
    }
}

string readNonEmptyLine(const string &prompt) {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        input = trim(input);
        if (!input.empty()) {
            return input;
        }
        cout << "Input cannot be empty. Please try again.\n";
    }
}

string sanitizeForFilename(const string &value) {
    string result;
    for (char c : value) {
        if (isalnum(static_cast<unsigned char>(c))) {
            result.push_back(c);
        } else if (c == '-' || c == '_') {
            result.push_back(c);
        }
    }
    if (result.empty()) {
        result = "SESSION";
    }
    return result;
}

string buildSessionFilename(const AttendanceSession &session) {
    string code = sanitizeForFilename(toUpper(session.courseCode));
    string date = session.date;
    replace(date.begin(), date.end(), '-', '_');
    return "session_" + code + "_" + date + ".txt";
}

bool studentExists(const vector<Student> &students, const string &indexNumber) {
    return any_of(students.begin(), students.end(), [&](const Student &student) {
        return student.indexNumber == indexNumber;
    });
}

int findSessionIndex(const vector<AttendanceSession> &sessions, const string &courseCode, const string &date) {
    string normalizedCode = toUpper(trim(courseCode));
    string normalizedDate = trim(date);

    for (size_t i = 0; i < sessions.size(); ++i) {
        if (toUpper(sessions[i].courseCode) == normalizedCode && sessions[i].date == normalizedDate) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void saveStudentsToFile(const vector<Student> &students) {
    ofstream outFile(STUDENTS_FILE);
    if (!outFile) {
        cerr << "Warning: Could not write to " << STUDENTS_FILE << "\n";
        return;
    }

    for (const Student &student : students) {
        outFile << student.indexNumber << "|" << student.fullName << "|" << student.programme << "\n";
    }
}

void saveSingleSession(const AttendanceSession &session, const string &filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Warning: Could not write to " << filename << "\n";
        return;
    }

    outFile << session.courseCode << "\n";
    outFile << session.date << "\n";
    outFile << session.startTime << "\n";
    outFile << session.durationMinutes << "\n";

    for (const AttendanceRecord &record : session.records) {
        outFile << record.studentIndex << "|" << record.status << "\n";
    }
}

void saveSessionsToFiles(const vector<AttendanceSession> &sessions) {
    ofstream indexFile(SESSIONS_INDEX_FILE);
    if (!indexFile) {
        cerr << "Warning: Could not write to " << SESSIONS_INDEX_FILE << "\n";
        return;
    }

    for (const AttendanceSession &session : sessions) {
        string filename = buildSessionFilename(session);
        indexFile << filename << "\n";
        saveSingleSession(session, filename);
    }
}

vector<Student> loadStudentsFromFile() {
    vector<Student> students;
    ifstream inFile(STUDENTS_FILE);

    if (!inFile) {
        return students;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string index, name, programme;

        getline(ss, index, '|');
        getline(ss, name, '|');
        getline(ss, programme, '|');

        index = trim(index);
        name = trim(name);
        programme = trim(programme);

        if (!index.empty() && !name.empty()) {
            students.push_back({index, name, programme});
        }
    }

    return students;
}

bool loadSingleSession(const string &filename, AttendanceSession &session) {
    ifstream inFile(filename);
    if (!inFile) {
        return false;
    }

    if (!getline(inFile, session.courseCode)) return false;
    if (!getline(inFile, session.date)) return false;
    if (!getline(inFile, session.startTime)) return false;

    string durationLine;
    if (!getline(inFile, durationLine)) return false;

    stringstream durationStream(durationLine);
    if (!(durationStream >> session.durationMinutes)) {
        return false;
    }

    session.records.clear();

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string index, status;
        getline(ss, index, '|');
        getline(ss, status, '|');

        index = trim(index);
        status = trim(status);

        if (!index.empty() && !status.empty()) {
            session.records.push_back({index, status});
        }
    }

    return true;
}

vector<AttendanceSession> loadSessionsFromFiles() {
    vector<AttendanceSession> sessions;
    ifstream indexFile(SESSIONS_INDEX_FILE);

    if (!indexFile) {
        return sessions;
    }

    string filename;
    while (getline(indexFile, filename)) {
        filename = trim(filename);
        if (filename.empty()) {
            continue;
        }

        AttendanceSession session;
        if (loadSingleSession(filename, session)) {
            sessions.push_back(session);
        }
    }

    return sessions;
}

void registerStudent(vector<Student> &students) {
    cout << "\n--- Register Student ---\n";

    string indexNumber = readNonEmptyLine("Enter index number: ");

    if (studentExists(students, indexNumber)) {
        cout << "Student with index number " << indexNumber << " already exists.\n";
        return;
    }

    string fullName = readNonEmptyLine("Enter full name: ");
    string programme = readNonEmptyLine("Enter programme: ");

    students.push_back({indexNumber, fullName, programme});
    cout << "Student registered successfully.\n";
}

void viewAllStudents(const vector<Student> &students) {
    cout << "\n--- Registered Students ---\n";

    if (students.empty()) {
        cout << "No students registered yet.\n";
        return;
    }

    cout << left << setw(15) << "Index Number" << setw(35) << "Name" << "Programme\n";
    cout << string(70, '-') << "\n";
    for (const Student &student : students) {
        cout << left << setw(15) << student.indexNumber
             << setw(35) << student.fullName
             << student.programme << "\n";
    }
}

void searchStudentByIndex(const vector<Student> &students) {
    cout << "\n--- Search Student ---\n";

    string indexNumber = readNonEmptyLine("Enter index number to search: ");

    for (const Student &student : students) {
        if (student.indexNumber == indexNumber) {
            cout << "Student found:\n";
            cout << "Index Number: " << student.indexNumber << "\n";
            cout << "Full Name: " << student.fullName << "\n";
            cout << "Programme: " << student.programme << "\n";
            return;
        }
    }

    cout << "No student found with index number " << indexNumber << ".\n";
}

void createLectureSession(vector<AttendanceSession> &sessions) {
    cout << "\n--- Create Lecture Session ---\n";

    AttendanceSession session;
    session.courseCode = toUpper(readNonEmptyLine("Enter course code (e.g., EEE227): "));

    while (true) {
        session.date = readNonEmptyLine("Enter date (YYYY-MM-DD): ");
        if (isValidDate(session.date)) {
            break;
        }
        cout << "Invalid date format. Example: 2026-02-24\n";
    }

    while (true) {
        session.startTime = readNonEmptyLine("Enter start time (HH:MM): ");
        if (isValidTime(session.startTime)) {
            break;
        }
        cout << "Invalid time format. Example: 09:30\n";
    }

    session.durationMinutes = readInt("Enter duration in minutes (1-600): ", 1, 600);

    if (findSessionIndex(sessions, session.courseCode, session.date) != -1) {
        cout << "A session for " << session.courseCode << " on " << session.date << " already exists.\n";
        return;
    }

    sessions.push_back(session);
    cout << "Session created successfully.\n";
}

void listSessions(const vector<AttendanceSession> &sessions) {
    cout << "\n--- Available Sessions ---\n";
    if (sessions.empty()) {
        cout << "No sessions created yet.\n";
        return;
    }

    for (size_t i = 0; i < sessions.size(); ++i) {
        const AttendanceSession &session = sessions[i];
        cout << i + 1 << ". " << session.courseCode << " | " << session.date
             << " | " << session.startTime << " | " << session.durationMinutes << " mins\n";
    }
}

int pickSession(const vector<AttendanceSession> &sessions) {
    if (sessions.empty()) {
        cout << "No sessions available. Create a session first.\n";
        return -1;
    }

    listSessions(sessions);
    int selection = readInt("Choose session number: ", 1, static_cast<int>(sessions.size()));
    return selection - 1;
}

void markAttendance(vector<AttendanceSession> &sessions, const vector<Student> &students) {
    cout << "\n--- Mark Attendance ---\n";

    if (students.empty()) {
        cout << "No registered students. Add students first.\n";
        return;
    }

    int sessionIndex = pickSession(sessions);
    if (sessionIndex < 0) {
        return;
    }

    AttendanceSession &session = sessions[sessionIndex];

    for (const Student &student : students) {
        cout << "Student: " << student.indexNumber << " - " << student.fullName << "\n";
        cout << "Enter status (Present/Absent/Late): ";

        string status;
        getline(cin, status);
        while (!isValidStatus(status)) {
            cout << "Invalid status. Enter Present, Absent, or Late: ";
            getline(cin, status);
        }

        status = normalizeStatus(status);

        auto it = find_if(session.records.begin(), session.records.end(), [&](const AttendanceRecord &record) {
            return record.studentIndex == student.indexNumber;
        });

        if (it == session.records.end()) {
            session.records.push_back({student.indexNumber, status});
        } else {
            it->status = status;
        }
    }

    cout << "Attendance marked successfully.\n";
}

void updateAttendanceRecord(vector<AttendanceSession> &sessions, const vector<Student> &students) {
    cout << "\n--- Update Attendance Record ---\n";

    int sessionIndex = pickSession(sessions);
    if (sessionIndex < 0) {
        return;
    }

    AttendanceSession &session = sessions[sessionIndex];
    string indexNumber = readNonEmptyLine("Enter student index number: ");

    if (!studentExists(students, indexNumber)) {
        cout << "Student not found in registered list.\n";
        return;
    }

    auto it = find_if(session.records.begin(), session.records.end(), [&](const AttendanceRecord &record) {
        return record.studentIndex == indexNumber;
    });

    if (it == session.records.end()) {
        cout << "No attendance record exists for this student in selected session.\n";
        return;
    }

    cout << "Current status: " << it->status << "\n";
    cout << "Enter new status (Present/Absent/Late): ";

    string newStatus;
    getline(cin, newStatus);

    while (!isValidStatus(newStatus)) {
        cout << "Invalid status. Enter Present, Absent, or Late: ";
        getline(cin, newStatus);
    }

    it->status = normalizeStatus(newStatus);
    cout << "Attendance updated successfully.\n";
}

void displayAttendanceListForSession(const vector<AttendanceSession> &sessions, const vector<Student> &students) {
    cout << "\n--- Attendance List ---\n";

    int sessionIndex = pickSession(sessions);
    if (sessionIndex < 0) {
        return;
    }

    const AttendanceSession &session = sessions[sessionIndex];
    cout << "Session: " << session.courseCode << " | " << session.date << " | " << session.startTime << "\n";

    if (session.records.empty()) {
        cout << "No attendance has been marked for this session.\n";
        return;
    }

    cout << left << setw(15) << "Index Number" << setw(35) << "Name" << "Status\n";
    cout << string(65, '-') << "\n";

    for (const AttendanceRecord &record : session.records) {
        string name = "Unknown Student";
        for (const Student &student : students) {
            if (student.indexNumber == record.studentIndex) {
                name = student.fullName;
                break;
            }
        }

        cout << left << setw(15) << record.studentIndex << setw(35) << name << record.status << "\n";
    }
}

void displayAttendanceSummary(const vector<AttendanceSession> &sessions) {
    cout << "\n--- Attendance Summary ---\n";

    int sessionIndex = pickSession(sessions);
    if (sessionIndex < 0) {
        return;
    }

    const AttendanceSession &session = sessions[sessionIndex];
    int presentCount = 0;
    int absentCount = 0;
    int lateCount = 0;

    for (const AttendanceRecord &record : session.records) {
        if (record.status == "Present") {
            ++presentCount;
        } else if (record.status == "Absent") {
            ++absentCount;
        } else if (record.status == "Late") {
            ++lateCount;
        }
    }

    cout << "Session: " << session.courseCode << " | " << session.date << "\n";
    cout << "Present: " << presentCount << "\n";
    cout << "Absent : " << absentCount << "\n";
    cout << "Late   : " << lateCount << "\n";
    cout << "Total Marked: " << session.records.size() << "\n";
}

void saveAllData(const vector<Student> &students, const vector<AttendanceSession> &sessions) {
    saveStudentsToFile(students);
    saveSessionsToFiles(sessions);
    cout << "Data saved successfully.\n";
}

void showMenu() {
    cout << "\n==========================================\n";
    cout << " DIGITAL ATTENDANCE SYSTEM - EEE227\n";
    cout << "==========================================\n";
    cout << "1. Register student\n";
    cout << "2. View all students\n";
    cout << "3. Search student by index number\n";
    cout << "4. Create lecture session\n";
    cout << "5. Mark attendance\n";
    cout << "6. Update attendance record\n";
    cout << "7. Display attendance list for a session\n";
    cout << "8. Display attendance summary\n";
    cout << "9. Save data\n";
    cout << "0. Exit\n";
}

int main() {
    vector<Student> students = loadStudentsFromFile();
    vector<AttendanceSession> sessions = loadSessionsFromFiles();

    cout << "Welcome to the Digital Attendance System (EEE227).\n";
    cout << students.size() << " student(s) loaded, " << sessions.size() << " session(s) loaded.\n";

    while (true) {
        showMenu();
        int choice = readInt("Select an option: ", 0, 9);

        switch (choice) {
            case 1:
                registerStudent(students);
                break;
            case 2:
                viewAllStudents(students);
                break;
            case 3:
                searchStudentByIndex(students);
                break;
            case 4:
                createLectureSession(sessions);
                break;
            case 5:
                markAttendance(sessions, students);
                break;
            case 6:
                updateAttendanceRecord(sessions, students);
                break;
            case 7:
                displayAttendanceListForSession(sessions, students);
                break;
            case 8:
                displayAttendanceSummary(sessions);
                break;
            case 9:
                saveAllData(students, sessions);
                break;
            case 0:
                saveAllData(students, sessions);
                cout << "Exiting program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
                break;
        }
    }
}
