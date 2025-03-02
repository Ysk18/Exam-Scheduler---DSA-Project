#include "schedule.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
using namespace std;

// Create a new schedule with 7 days
struct Schedule *CreateSchedule()
{
    // TODO: Implement creation of a schedule with 7 days (circular list)
    Schedule *schedule = new Schedule();

    struct Day *Monday = (Day *)malloc(sizeof(Day));
    strcpy(Monday->dayName, "Monday");
    struct Day *Tuesday = (Day *)malloc(sizeof(Day));
    strcpy(Tuesday->dayName, "Tuesday");
    struct Day *Wednesday = (Day *)malloc(sizeof(Day));
    strcpy(Wednesday->dayName, "Wednesday");
    struct Day *Thursday = (Day *)malloc(sizeof(Day));
    strcpy(Thursday->dayName, "Thursday");
    struct Day *Friday = (Day *)malloc(sizeof(Day));
    strcpy(Friday->dayName, "Friday");
    struct Day *Saturday = (Day *)malloc(sizeof(Day));
    strcpy(Saturday->dayName, "Saturday");
    struct Day *Sunday = (Day *)malloc(sizeof(Day));
    strcpy(Sunday->dayName, "Sunday");

    Monday->nextDay = Tuesday;
    Tuesday->nextDay = Wednesday;
    Wednesday->nextDay = Thursday;
    Thursday->nextDay = Friday;
    Friday->nextDay = Saturday;
    Saturday->nextDay = Sunday;
    Sunday->nextDay = Monday;

    schedule->head = Monday;

    cout << "Schedule creation complete." << "\n";

    return schedule;
}
bool isConflict(struct Schedule *schedule, const char *day, int startTime, int endTime)
{
    struct Day *today = schedule->head;
    while (strcmp(today->dayName, day) != 0)
    {
        today = today->nextDay;
    }
    struct Exam *todaysExams = today->examList;
    int start = 8;
    int end = 9;
    while (todaysExams != NULL)
    {
        end = todaysExams->startTime;
        if (startTime < end && startTime > start && endTime > start && endTime < end)
        {
            return true;
        }
    }
    end = 20;
    if (startTime < end && startTime > start && endTime > start && endTime < end)
    {
        return true;
    }
    return false;
}

// Add an exam to a day in the schedule
int AddExamToSchedule(struct Schedule *schedule, const char *day, int startTime, int endTime, const char *courseCode)
{
    int size = endTime - startTime;
    if (size > 3 || size < 1 || startTime < 8 || startTime > 17 || endTime < 9 || endTime > 20)
    {
        cout << "Invalid exam." << "\n";
        return 3;
    }

    struct Exam *newExam = CreateExam(startTime, endTime, courseCode);
    newExam->next = NULL;

    struct Day *today = schedule->head; // equals that day
    while (strcmp(today->dayName, day) != 0)
    {
        today = today->nextDay;
    }

    int count = 0;
    int start = 8;
    int end = 9;
    bool isPlacedToCorrectPlace = true;
    bool isPlaced = false;

    struct Exam *todaysExams = today->examList;
    struct Exam *prevExam = NULL;

    while (count < 7)
    {
        todaysExams = today->examList;
        if (todaysExams == NULL)
        {
            today->examList = newExam;
            if (strcmp(today->dayName, day) == 0)
            {
                cout << courseCode << " exam added to " << today->dayName << " at time " << startTime << " to " << endTime << " without conflict." << "\n";
                return 0;
            }
            else
            {
                cout << courseCode << " exam added to " << today->dayName << " at time " << newExam->startTime << " to " << newExam->endTime << " due to conflict." << "\n";
                return 1;
            }
            isPlaced = true;
        }
        while (todaysExams != NULL)
        {
            end = todaysExams->startTime;
            if (isPlacedToCorrectPlace)
            {
                if (startTime >= start && startTime < end)
                {
                    if (endTime <= end) // placed correctly
                    {
                        struct Exam *tmpExamPointer = todaysExams->next;
                        todaysExams->next = newExam;
                        newExam->next = tmpExamPointer;
                        cout << courseCode << " exam added to " << day << " at time " << startTime << " to " << endTime << " without conflict." << "\n";
                        isPlaced = true;
                        return 0;
                    }
                    else
                    {
                        isPlacedToCorrectPlace = false;
                    }
                }
                else
                {
                    if (todaysExams != NULL)
                        start = todaysExams->endTime;
                    prevExam = todaysExams;
                    todaysExams = todaysExams->next;
                    continue;
                }
            }
            if (size <= end - start) // başlangıç ve sonu ayrı durumlarda değerlendireceğim
            {
                newExam->startTime = start;
                newExam->endTime = start + size;
                struct Exam *tmpExamPointer = todaysExams->next;
                todaysExams->next = newExam;
                newExam->next = tmpExamPointer;
                cout << courseCode << " exam added to " << today->dayName << " at time " << newExam->startTime << " to " << newExam->endTime << " due to conflict." << "\n";
                isPlaced = true;
                return 1;
            }
            if (todaysExams != NULL)
                start = todaysExams->endTime;
            prevExam = todaysExams;
            todaysExams = todaysExams->next;
        }
        end = 20;
        if (!isPlaced && size <= end - start)
        {
            newExam->startTime = start;
            newExam->endTime = start + size;
            if (prevExam != NULL)
            {
                prevExam->next = newExam;
            }
            else
            {
                today->examList = newExam;
            }
            cout << courseCode << " exam added to " << today->dayName << " at time " << newExam->startTime << " to " << newExam->endTime << " due to conflict." << "\n";
            return 1;
        }

        count++;
        today = today->nextDay;
        start = 8;
    }

    cout << "Schedule full. Exam cannot be added." << "\n";
    free(todaysExams);
    free(prevExam);
    return 2;
}

// Remove an exam from a specific day in the schedule
int RemoveExamFromSchedule(struct Schedule *schedule, const char *day, int startTime)
{
    struct Day *today = schedule->head; // equals that day
    while (strcmp(today->dayName, day) != 0)
    {
        today = today->nextDay;
    }
    struct Exam *examListToday = today->examList;
    struct Exam *prevExam = NULL;
    while (examListToday != NULL)
    {
        if (startTime == examListToday->startTime)
        {
            if (prevExam == NULL)
            {
                if (today->examList->next == NULL)
                    today->examList = NULL;
                else
                    today->examList = today->examList->next;
            }
            else
            {
                prevExam->next = examListToday->next;
            }
            free(examListToday);
            cout << "Exam removed succesfully.\n";
            free(prevExam);
            return 0;
        }
        prevExam = examListToday;
        examListToday = examListToday->next;
    }

    cout << "Exam could not be found\n";
    free(today);
    free(examListToday);
    free(prevExam);
    return 1;
}

// Update an exam in the schedule
int UpdateExam(struct Schedule *schedule, const char *oldDay, int oldStartTime, const char *newDay, int newStartTime, int newEndTime)
{
    int size = newEndTime - newStartTime;
    if (size > 3 || size < 1 || newStartTime < 8 || newStartTime > 17 || newEndTime < 9 || newEndTime > 20)
    {
        cout << "Invalid exam." << "\n";
        return 3;
    }

    struct Day *today = schedule->head; // equals that day
    while (strcmp(today->dayName, oldDay) != 0)
    {
        today = today->nextDay;
    }
    struct Exam *examListToday = today->examList;
    struct Exam *prevExam = NULL;
    if (examListToday != NULL)
    {
        while (examListToday != NULL && examListToday->startTime != oldStartTime)
        {
            prevExam = examListToday;
            examListToday = examListToday->next;
        }
    }
    if (examListToday == NULL)
    {
        cout << "Exam could not be found.\n";
        return 2;
    }
    int start = 8;
    if (prevExam != NULL)
        prevExam->endTime;
    int end;
    if (examListToday->next != NULL)
    {
        end = examListToday->next->startTime;
    }
    else
    {
        end = 20;
    }

    if (strcmp(today->dayName, newDay) == 0)
    {
        if (newStartTime >= start && newStartTime < end && newEndTime > start && newEndTime <= end)
        {
            examListToday->startTime = newStartTime;
            examListToday->endTime = newEndTime;
            cout << "Update succesfull.\n";
            return 0;
        }
    }

    struct Exam *newExam = CreateExam(newStartTime, newEndTime, examListToday->courseCode);
    newExam->next = NULL;

    today = schedule->head; // equals that day
    while (strcmp(today->dayName, newDay) != 0)
    {
        today = today->nextDay;
    }

    start = 8;
    end = 9;
    bool isPlacedToCorrectPlace = true;
    bool isPlaced = false;

    struct Exam *todaysExams = today->examList;
    prevExam = NULL;

    if (todaysExams == NULL)
    {
        today->examList = newExam;
        if (strcmp(today->dayName, newDay) == 0)
        {
            cout << "Update successfull.\n";
            return 0;
        }
        else
        {
            cout << "Update unsuccessfull.\n";
            return 1;
        }
        isPlaced = true;
    }
    while (todaysExams != NULL)
    {
        end = todaysExams->startTime;
        if (isPlacedToCorrectPlace)
        {
            if (newStartTime >= start && newStartTime < end)
            {
                if (newEndTime <= end) // placed correctly
                {
                    struct Exam *tmpExamPointer = todaysExams->next;
                    todaysExams->next = newExam;
                    newExam->next = tmpExamPointer;
                    cout << "Update successfull.\n";
                    struct Day *today2 = schedule->head; // equals that day
                    while (strcmp(today2->dayName, oldDay) != 0)
                    {
                        today2 = today2->nextDay;
                    }
                    struct Exam *examListToday2 = today2->examList;
                    prevExam = NULL;
                    while (examListToday2 != NULL)
                    {
                        if (oldStartTime == examListToday2->startTime)
                        {
                            if (prevExam == NULL)
                            {
                                if (today2->examList->next == NULL)
                                    today2->examList = NULL;
                                else
                                    today2->examList = today2->examList->next;
                            }
                            else
                            {
                                prevExam->next = examListToday2->next;
                            }
                            free(examListToday2);
                            free(prevExam);
                            return 0;
                        }
                        prevExam = examListToday2;
                        examListToday2 = examListToday2->next;
                    }
                    return 0;
                }
                else
                {
                    isPlacedToCorrectPlace = false;
                }
            }
            else
            {
                if (todaysExams != NULL)
                    start = todaysExams->endTime;
                prevExam = todaysExams;
                todaysExams = todaysExams->next;
                continue;
            }
        }
        if (size >= end - start) // başlangıç ve sonu ayrı durumlarda değerlendireceğim
        {
            cout << "Update unsuccessfull.\n";
            isPlaced = true;
            return 1;
        }
        if (todaysExams != NULL)
            start = todaysExams->endTime;
        prevExam = todaysExams;
        todaysExams = todaysExams->next;
    }
    end = 20;
    if (!isPlaced && size <= end - start)
    {
        cout << "Update unsuccessfull.\n";
        return 1;
    }

    cout << "Update unsuccessfull.\n";
    return 1;
}

// Clear all exams from a specific day and relocate them to other days
int ClearDay(struct Schedule *schedule, const char *day)
{
    return 0;
}

// Clear all exams and days from the schedule and deallocate memory
void DeleteSchedule(struct Schedule *schedule)
{
    int count = 0;
    struct Day *today = schedule->head;
    while (count < 7)
    {
        struct Exam *todaysExams = today->examList;
        while (todaysExams != NULL)
        {
            struct Exam *tmpExam = todaysExams;
            todaysExams = todaysExams->next;
            free(tmpExam);
        }
        struct Day *tmpToday = today;
        today = today->nextDay;
        free(tmpToday);
        count++;
    }
    free(schedule);
}

int ReadScheduleFromFile(Schedule *schedule, const char *filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return -1;

    std::string line;
    Day *lastDay = nullptr;

    while (std::getline(file, line))
    {
        if (!line.empty() && !isdigit(line[0]))
        {
            Day *day = new Day(line);
            if (!schedule->head)
            {
                schedule->head = day;
                lastDay = day;
            }
            else
            {
                lastDay->nextDay = day;
                lastDay = day;
            }
        }
        else
        {
            std::stringstream ss(line);
            int start, end;
            std::string code;
            ss >> start >> end >> code;

            if (lastDay)
            {
                Exam *exam = new Exam(start, end, code);
                exam->next = lastDay->examList;
                lastDay->examList = exam;
            }
        }
    }
    if (lastDay)
        lastDay->nextDay = schedule->head;
    file.close();
    return 0;
}

int WriteScheduleToFile(Schedule *schedule, const char *filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
        return -1;

    Day *day = schedule->head;
    do
    {
        file << day->dayName << "\n";
        Exam *exam = day->examList;
        while (exam)
        {
            file << exam->startTime << " " << exam->endTime << " " << exam->courseCode << "\n";
            exam = exam->next;
        }
        file << "(No exams scheduled)\n";
        day = day->nextDay;
    } while (day != schedule->head);

    file.close();
    return 0;
}

/*
// Read schedule from a file
int ReadScheduleFromFile(Schedule* schedule, const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return -1; // File not found or unable to open
    }

    // TODO: Implement reading the schedule from file
    infile.close();
    return 0; // Success
}

// Write schedule to a file
int WriteScheduleToFile(Schedule* schedule, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return -1; // File could not be opened
    }

    // TODO: Implement writing the schedule to file
    outfile.close();
    return 0; // Success
}
*/