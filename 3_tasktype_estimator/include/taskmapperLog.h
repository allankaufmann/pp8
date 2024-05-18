


#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERLOG_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERLOG_H
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include "taskclasses.h"
#include <fstream>
extern std::vector<AnwTask> apptaskVektor;
extern std::map<std::string, AnwTask> apptaskMap; // Abbildung der Anwendungstask auf Map
extern std::ofstream logfileTaskmapper;
extern PrototypTask currentPrototypTask;//für Diff/Paarsuche
extern char* currentLogfileName;
extern std::vector<PrototypTask> prottaskVektor;
/**
 * Für die Abbildung 1:N werden die Tasks parallel geprüft. Aus diesem Grund werden die Zwischenergebnisse hier zusammengeführt.
 */
extern Result resultOneToMany;
void logMessageOnTaskmapperFileAndCout(std::string text, bool withLogFileTaskmapper);
void initTaskVektors();
bool checkSequenzfiles();
void openLogfileTaskmapper();
void logBestTasks(AnwTask appTask);
AnwTask logBestTask(AnwTask appTask, bool percent, bool savetofile);
void closeLogFileTaskmapper();
std::vector<std::string> readSeqFile(const char* file, Task t);
PrototypTask readProttypTaskSeqfile(const char* file);
AnwTask readAnwTaskSeqfile(const char* file);
void initProttaskVektor();
void initAppTaskCollections();
void dropModelInTaskmapFile(std::string model);
void saveLineToTaskmapFile(std::string sectionToUpdate, std::string newLineToUpdate);
void transferTaskMapToEpEBench();
#endif //INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERLOG_H
