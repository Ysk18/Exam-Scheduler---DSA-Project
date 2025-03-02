#include "exam.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a new exam and link it properly in the day
struct Exam *CreateExam(int startTime, int endTime, const char *courseCode)
{
    struct Exam *newExam = (Exam *)malloc(sizeof(Exam));
    newExam->startTime = startTime;
    newExam->endTime = endTime;
    newExam->next = NULL;
    strcpy(newExam->courseCode, courseCode);

    return newExam;
}