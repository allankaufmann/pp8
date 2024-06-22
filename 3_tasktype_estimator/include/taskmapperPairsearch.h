

#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERPAIRSEARCH_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERPAIRSEARCH_H
#include <vector>
#include "../include/taskclasses.h"
#include "../include/taskmapperLog.h"
#include <iostream>


// Vector of new prototype tasks
extern std::vector<PrototypTask> newprottaskVektor;

/**
 * Function to analyze a prototype task
 * @param prototypTask - Prototype task to analyze
 * @return PrototypTask - Analyzed prototype task
 */
PrototypTask analyseTaskType(PrototypTask prototypTask);

/**
 * Function to analyze an application task for pair search
 * @param anwTak - Application task to analyze
 * @return AnwTask - Analyzed application task
 */
AnwTask analyseAppTaskPair(AnwTask anwTak);

/**
 * Function to test the pair search functionality
 */
void testPairSearch();

/**
 * Function to test the triple search functionality
 */
void testTripleSearch();

#endif //INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERPAIRSEARCH_H
