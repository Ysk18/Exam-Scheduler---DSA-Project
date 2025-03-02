#ifndef EXAM_H
#define EXAM_H
#include <string>

// Structure definition for Exam (this should be in "exam.h")
struct Exam {
    int startTime;
    int endTime;
    char courseCode[50];  // Fixed-size char array for storing course code
    struct Exam* next;
};

// Function to create a new exam and link it properly in the day
Exam* CreateExam(int startTime, int endTime, const std::string& courseCode);

// Helper function to print an exam
void PrintExam(Exam* exam);

#endif // EXAM_H
