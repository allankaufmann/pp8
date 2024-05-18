//
// Created by allan on 18.05.24.
//

#ifndef EXPERIMENT_TASKMAPPERDIFFSEARCH_H
#define EXPERIMENT_TASKMAPPERDIFFSEARCH_H
#include "../include/taskclasses.h"
#include <vector>
#include <string>
#include "../include/taskmapperLog.h"
extern std::vector<std::string> similarTasks;
PrototypTask compareProtTaskSequenEntries(int indexToCompare, PrototypTask protTypTaskToCompare);
void comparePrototypTaskWithOtherPrototypTask(PrototypTask protTypTaskToCompare);
bool isSimilarTask(std::string taskname);
void testDiffSearch();
#endif //EXPERIMENT_TASKMAPPERDIFFSEARCH_H
