
#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOMANY_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOMANY_H
#include <string.h>
#include "../include/Result.h"
#include "../include/taskclasses.h"
#include "../include/taskmapperLog.h"
#include <thread>
#include "math.h" //ceil


/**
 * Function to calculate the best task for many-to-one mapping
 * @param appTask - Application task to consider
 * @return AnwTask - Best task after calculation
 */
AnwTask calcBestTaskMany(AnwTask appTask);

/**
 * Function to calculate the size of the sequence
 * @return float - Size of the sequence
 */
float calcSequenzSize();

/**
 * Function to compare a prototype task sequence entry with an application task entry for many-to-one mapping
 * @param protTaskSequenceEntry - Prototype task sequence entry to compare
 * @param appTask - Application task to compare with
 * @param protTypTask - Prototype task to consider
 * @param maxIndex - Maximum index to consider
 * @return AnwTask - Application task after comparison
 */
AnwTask compareProtTaskSequenEntryWithAppTaskEntryMany(std::string protTaskSequenceEntry, AnwTask appTask, PrototypTask protTypTask, int maxIndex);

/**
 * Function to compare an application task with prototype tasks for many-to-one mapping
 * @param appTask - Application task to compare
 * @param protTypTask - Prototype task to compare with
 * @param maxIndex - Maximum index to consider
 * @return AnwTask - Application task after comparison
 */
AnwTask compareAppTaskWithPrototypTasksMany(AnwTask appTask, PrototypTask protTypTask, int maxIndex );

/**
 * Function to prepare an application task and a prototype task for comparison
 * @param appTask - Application task to prepare
 * @param protTypTask - Prototype task to prepare
 * @return AnwTask - Prepared application task
 */
AnwTask prepareAnwTaskAndProtTypTaskForCompare(AnwTask appTask, PrototypTask protTypTask );

/**
 * Function to analyze an application task
 * @param appTask - Application task to analyze
 * @return AnwTask - Analyzed application task
 */
AnwTask analyse(AnwTask appTask);

/**
 * Function to analyze an application task for many-to-one mapping
 * @param appTask - Application task to analyze
 * @return AnwTask - Analyzed application task
 */
AnwTask analyseAppTaskMany(AnwTask appTask);

/**
 * Function to compare an application task with prototype tasks for one-to-many mapping
 */
void compareAppTaskProtTasksOneToMany();

/**
 * Function to test the one-to-many mapping functionality
 * @param appTaskName - Name of the application task to test
 */
void compareAppTaskProtTasksOneToManyTest(std::string appTaskName);



#endif //INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOMANY_H
