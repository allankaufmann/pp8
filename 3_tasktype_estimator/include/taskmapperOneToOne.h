//
// Created by allan on 16.05.24.
//


#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOONE_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOONE_H
#include "../include/Result.h"
#include "../include/taskclasses.h"
#include "../include/taskmapperLog.h"
#include <thread>

// Temporary result for one-to-one mapping
extern Result tempResultOneToOne;

/**
 * Function to compare a prototype task sequence entry with an application task entry for one-to-one mapping
 * @param protTaskSequenceEntry - Prototype task sequence entry to compare
 * @param appTask - Application task to compare with
 * @param maxIndex - Maximum index to consider
 * @return AnwTask - Application task after comparison
 */
AnwTask compareProtTaskSequenEntryWithAppTaskEntry(std::string protTaskSequenceEntry, AnwTask appTask, int maxIndex);

/**
 * Function to log the one-to-one task mapper
 * @param taskname - Name of the task
 * @param ptname - Name of the prototype task
 * @param size - Size of the task
 * @param sizeAppTask - Size of the application task
 * @param sizeProtTask - Size of the prototype task
 * @param div - Division factor
 */
void logTaskMapperOne2One(std::string taskname, std::string ptname, int size, int sizeAppTask, int sizeProtTask, int div);


/**
 * Function to compare an application task with prototype tasks for one-to-one mapping
 * @param appTask - Application task to compare
 * @param protTypTask - Prototype task to compare with
 * @return AnwTask - Application task after comparison
 */
AnwTask compareAppTaskWithPrototypTasks(AnwTask appTask, PrototypTask protTypTask );

/**
 * Function to prepare an application task and a prototype task for comparison for one-to-one mapping
 * @param appTask - Application task to prepare
 * @param protTypTask - Prototype task to prepare
 * @return AnwTask - Prepared application task
 */
AnwTask prepareAnwTaskAndProtTypTaskForCompareOneToOne(AnwTask appTask, PrototypTask protTypTask );

/**
 * Function to analyze an application task for one-to-one mapping
 * @param appTask - Application task to analyze
 * @return AnwTask - Analyzed application task
 */
AnwTask analyseAppTask(AnwTask appTask);

/**
 * Function to compare an application task with prototype tasks for one-to-one mapping
 */
void compareAppTaskProtTasksOneToOne();

/**
 * Function to test the one-to-one mapping functionality
 * @param appTaskName - Name of the application task to test
 */
void compareAppTaskProtTasksOneToOneTest(std::string appTaskName);
#endif //INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOONE_H
