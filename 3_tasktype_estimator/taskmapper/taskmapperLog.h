
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include "taskclasses.h"
#include <fstream>

#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERLOG_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERLOG_H
std::vector<PrototypTask> prottaskVektor;
std::vector<AnwTask> apptaskVektor;
std::map<std::string, AnwTask> apptaskMap; // Abbildung der Anwendungstask auf Map
std::ofstream logfileTaskmapper;
PrototypTask currentPrototypTask;//für Diff/Paarsuche
/**
 * Für die Abbildung 1:N werden die Tasks parallel geprüft. Aus diesem Grund werden die Zwischenergebnisse hier zusammengeführt.
 */
Result resultOneToMany;
void logMessageOnTaskmapperFileAndCout(std::string text, bool withLogFileTaskmapper);
void initTaskVektors();
bool checkSequenzfiles();
void openLogfileTaskmapper();
void logBestTasks(AnwTask appTask);
AnwTask logBestTask(AnwTask appTask, bool percent, bool savetofile);
void closeLogFileTaskmapper();
#endif //INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERLOG_H
