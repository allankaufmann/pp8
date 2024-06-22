//
// Created by allan on 18.05.24.
//

#ifndef EXPERIMENT_TASKMAPPERDIFFSEARCH_H
#define EXPERIMENT_TASKMAPPERDIFFSEARCH_H
#include "../include/taskclasses.h"
#include <vector>
#include <string>
#include "../include/taskmapperLog.h"

// Vector to store the names of similar tasks
extern std::vector<std::string> similarTasks;

/**
 * Function to compare the sequences of a prototype task with another prototype task
 * @param indexToCompare - Index of the sequence to compare
 * @param protTypTaskToCompare - Prototype task to compare with
 * @return PrototypTask - The prototype task after comparison
 */
PrototypTask compareProtTaskSequenEntries(int indexToCompare, PrototypTask protTypTaskToCompare);

/**
 * Function to compare a prototype task with other prototype tasks
 * @param protTypTaskToCompare - Prototype task to compare with other prototype tasks
 */
void comparePrototypTaskWithOtherPrototypTask(PrototypTask protTypTaskToCompare);

/**
 * Function to check if a task is similar to other tasks
 * @param taskname - Name of the task to check
 * @return bool - True if the task is similar to other tasks, false otherwise
 */
bool isSimilarTask(std::string taskname);

/**
 * Function to test the difference search functionality
 */
void testDiffSearch();
#endif //EXPERIMENT_TASKMAPPERDIFFSEARCH_H
