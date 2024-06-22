


#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERLOG_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERLOG_H
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include "taskclasses.h"
#include <fstream>

// Vector of application tasks
extern std::vector<AnwTask> apptaskVektor;

// Map of application tasks
extern std::map<std::string, AnwTask> apptaskMap;

// Log file for task mapper
extern std::ofstream logfileTaskmapper;

// Current prototype task for difference/pair search
extern PrototypTask currentPrototypTask;

// Current log file name
extern char* currentLogfileName;

// Vector of prototype tasks
extern std::vector<PrototypTask> prottaskVektor;

// Result for one to many mapping
extern Result resultOneToMany;

// Function to log messages on taskmapper file and console
void logMessageOnTaskmapperFileAndCout(std::string text, bool withLogFileTaskmapper);

// Function to initialize task vectors
void initTaskVektors();

// Function to check sequence files
bool checkSequenzfiles();

// Function to open log file for task mapper
void openLogfileTaskmapper();

// Function to log best tasks
void logBestTasks(AnwTask appTask);

// Function to log best task
AnwTask logBestTask(AnwTask appTask, bool percent, bool savetofile);

// Function to close log file for task mapper
void closeLogFileTaskmapper();

// Function to read sequence file
std::vector<std::string> readSeqFile(const char* file, Task t);

// Function to read prototype task sequence file
PrototypTask readProttypTaskSeqfile(const char* file);

// Function to read application task sequence file
AnwTask readAnwTaskSeqfile(const char* file);

// Function to initialize prototype task vector
void initProttaskVektor();

// Function to initialize application task collections
void initAppTaskCollections();

// Function to drop model in task map file
void dropModelInTaskmapFile(std::string model);

// Function to save line to task map file
void saveLineToTaskmapFile(std::string sectionToUpdate, std::string newLineToUpdate);

// Function to transfer task map to epEBench
void transferTaskMapToEpEBench();

#endif //INC_3_TASKTYPE_ESTIMATOR_TASKMAPPERLOG_H
