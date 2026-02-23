# Digital Attendance System (EEE227) — Weekly Structured Submission

You are right: this project should be submitted **week by week**, not as one complicated block.

This repository is now arranged exactly by weekly basics:

```text
week1/main.cpp   -> Student class + add/view students
week2/main.cpp   -> AttendanceSession + menu flow
week3/main.cpp   -> Mark attendance + summary + better validation
week4/main.cpp   -> File save/load + final refactor + full system
```

---

## Week 1 (Basics)
**Scope required:**
- Create project structure
- Implement `Student`
- Add and view students

**Run:**
```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o week1_app week1/main.cpp
./week1_app
```

---

## Week 2
**Scope required:**
- Implement `AttendanceSession`
- Create lecture sessions
- Build menu-driven flow

**Run:**
```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o week2_app week2/main.cpp
./week2_app
```

---

## Week 3
**Scope required:**
- Implement attendance marking
- Add update/list/summary attendance options
- Improve input validation

**Run:**
```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o week3_app week3/main.cpp
./week3_app
```

---

## Week 4 (Final)
**Scope required:**
- File save/load using `fstream`
- Refactor and readability improvement
- Final integrated menu system

**Run:**
```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o attendance week4/main.cpp
./attendance
```

Data files used in final week:
- `students.txt`
- `sessions_index.txt`
- `session_<COURSE>_<DATE>.txt`

---

## Notes for Submission
- Commit work weekly on GitHub (minimum 3 meaningful commits per week as instructed).
- Demonstrate each week file separately during assessment.
- Use `week4/main.cpp` as the final capstone version.
