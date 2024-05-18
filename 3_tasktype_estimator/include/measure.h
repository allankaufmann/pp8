//
// Created by allan on 13.05.24.
//

#ifndef INC_3_TASKTYPE_ESTIMATOR_MEASURE_H
#define INC_3_TASKTYPE_ESTIMATOR_MEASURE_H
#include <string>
#include <fstream>
#include "MeasureResult.h"
extern std::ofstream logfileMeasure;
extern std::string currentCPUFreq;
extern std::string currentParallelism;

MeasureResult estimateAppTask(std::string apptaskname, std::string taskname, std::string cpufreq, std::string cores);
MeasureResult measureAppTask(std::string apptaskname, std::string cpufreq, std::string cores);
void logMeasureNewLine();
void logMeasureFlush();
void setupCpuFrequenzlevel(std::string frequenz);
void setCurrentParallelism(std::string parallel);
void smtoff();
void smton();
void openMeasurLogFile();
void closeMeasureLogFile();
uint64_t timeSinceEpochMillisec();
void logTime();
long long readCounterFromFile();
char* getFilename();
void logMeasure(const char app[], long long  dauer, long long energy_mj);
void logMeasure(MeasureResult res);
void logMeasureNewLine();
void logMeasureFlush();
void runCommand(const char* command);
MeasureResult runAndMeasureScript(const char* script);
void runAndMeasureScriptsFromDirectory(int count, const char* directory, const char* cores);
std::string  getFilenameWithParam(char* filename, std::string cores);
MeasureResult estimateAppTask(std::string apptaskname, std::string taskname, std::string cpufreq, std::string cores);
MeasureResult measureAppTask(std::string apptaskname, std::string cpufreq, std::string cores);
void selectCpuFrequency();
void measureAllPrototypTasks(int count);
#endif //INC_3_TASKTYPE_ESTIMATOR_MEASURE_H
