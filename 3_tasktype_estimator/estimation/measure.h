//
// Created by allan on 13.05.24.
//
#include <string>
#include <fstream>
#ifndef INC_3_TASKTYPE_ESTIMATOR_MEASURE_H
#define INC_3_TASKTYPE_ESTIMATOR_MEASURE_H

std::ofstream logfileMeasure;
std::string currentCPUFreq;
std::string currentParallelism;
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
#endif //INC_3_TASKTYPE_ESTIMATOR_MEASURE_H
