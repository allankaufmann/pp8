

#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERPAIRSEARCH_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERPAIRSEARCH_H
#include <vector>
#include "../include/taskclasses.h"
#include "../include/taskmapperLog.h"
#include <iostream>
extern std::vector<PrototypTask> newprottaskVektor;
PrototypTask analyseTaskType(PrototypTask prototypTask);
AnwTask analyseAppTaskPair(AnwTask anwTak);
void testPairSearch();
void testTripleSearch();

#endif //INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERPAIRSEARCH_H
