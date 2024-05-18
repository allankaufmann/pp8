
#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOMANY_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOMANY_H
#include <string.h>
#include "../include/Result.h"
#include "../include/taskclasses.h"
#include "../include/taskmapperLog.h"
#include <thread>
#include "math.h" //ceil
AnwTask calcBestTaskMany(AnwTask appTask);
float calcSequenzSize();
AnwTask compareProtTaskSequenEntryWithAppTaskEntryMany(std::string protTaskSequenceEntry, AnwTask appTask, PrototypTask protTypTask, int maxIndex);
AnwTask compareAppTaskWithPrototypTasksMany(AnwTask appTask, PrototypTask protTypTask, int maxIndex );
AnwTask prepareAnwTaskAndProtTypTaskForCompare(AnwTask appTask, PrototypTask protTypTask );
AnwTask analyse(AnwTask appTask);
AnwTask analyseAppTaskMany(AnwTask appTask);
void compareAppTaskProtTasksOneToMany();
void compareAppTaskProtTasksOneToManyTest(std::string appTaskName);



#endif //INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERONETOMANY_H
