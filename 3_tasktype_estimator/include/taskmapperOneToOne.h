//
// Created by allan on 16.05.24.
//


#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOONE_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOONE_H
#include "../include/Result.h"
#include "../include/taskclasses.h"
#include "../include/taskmapperLog.h"
#include <thread>
extern Result tempResultOneToOne;

/**
 * Überprüfung, ob Assemblerbefehl aus Prototyptask im Assemblercode der Anwendungstask vorkommt. Dabei werden bereits verwendete Codestellen übersprungen.
 *
 * @param protTaskSequenceEntry der gesuchte Assemblercode aus dem Prototyptask
 * @param appTask Taskobjekt des Anwendungstasks.
 * @return Anwendungstask nach Ende der Suche.
 */
AnwTask compareProtTaskSequenEntryWithAppTaskEntry(std::string protTaskSequenceEntry, AnwTask appTask, int maxIndex);


void logTaskMapperOne2One(std::string taskname, std::string ptname, int size, int sizeAppTask, int sizeProtTask, int div);


/**
 * Startet die eigentliche Ähnlichkeitssuche zwischen einem Anwendungstask und einem prototypischen Task.
 *
 *
 * @param appTask zu untersuchender Anwendungstask
 * @param protTypTask zu vergleichender prototypischer Task
 * @return liefert den Anwendungstask nach Untersuchung zurück
 */
AnwTask compareAppTaskWithPrototypTasks(AnwTask appTask, PrototypTask protTypTask );

AnwTask prepareAnwTaskAndProtTypTaskForCompareOneToOne(AnwTask appTask, PrototypTask protTypTask );

/**
 * Für den übergebenen Anwendungstask wird eine Ähnlichkeitssuche ausgeführt. Dabei werden nacheinander mit den vorhandenen Prototyptasks vergleichen.
 *
 * @param appTask zu untersuchender Anwendungstask.
 */
AnwTask analyseAppTask(AnwTask appTask);


void compareAppTaskProtTasksOneToOne();

void compareAppTaskProtTasksOneToOneTest(std::string appTaskName);
#endif //INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOONE_H
