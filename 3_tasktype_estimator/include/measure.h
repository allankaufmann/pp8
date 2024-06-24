//
// Created by allan on 13.05.24.
//

#ifndef INC_3_TASKTYPE_ESTIMATOR_MEASURE_H
#define INC_3_TASKTYPE_ESTIMATOR_MEASURE_H
#include <string>
#include <fstream>
#include "MeasureResult.h"
#include <list>
#include <map>

// Log file for measurements
extern std::ofstream logfileMeasure;

// Current CPU frequency
extern std::string currentCPUFreq;

// Current parallelism level
extern std::string currentParallelism;

// Map to store the measurement results
extern std::map<std::string, std::map<std::string, std::map<std::string, std::map<int, MeasureResult>>>> measureResultMap;

/**
 * Function to estimate application task
 * @param apptaskname - name of the application task
 * @param cpufreq - CPU frequency
 * @param cores - number of cores
 * @param repeat - number of times to repeat the estimation
 * @return MeasureResult - result of the measurement
 */
MeasureResult estimateAppTask(std::string apptaskname, std::string cpufreq, std::string cores, int repeat);

/**
 * Function to measure application task
 * @param apptaskname - name of the application task
 * @param cpufreq - CPU frequency
 * @param cores - number of cores
 * @return MeasureResult - result of the measurement
 */
MeasureResult measureAppTask(std::string apptaskname, std::string cpufreq, std::string cores);

// Function to log a new line in the measurement log file
void logMeasureNewLine();

// Function to flush the measurement log file
void logMeasureFlush();

/**
 * Function to set up CPU frequency level
 * @param frequenz - frequency level to set
 */
void setupCpuFrequenzlevel(std::string frequenz);

/**
 * Function to set current parallelism level
 * @param parallel - parallelism level to set
 */
void setCurrentParallelism(std::string parallel);

// Function to turn off simultaneous multithreading
void smtoff();

// Function to turn on simultaneous multithreading
void smton();

// Function to open the measurement log file
void openMeasurLogFile();

// Function to close the measurement log file
void closeMeasureLogFile();

// Function to get the current time since epoch in milliseconds
uint64_t timeSinceEpochMillisec();

// Function to get the filename
char* getFilename();

/**
 * Function to log measurement
 * @param app - application name
 * @param dauer - duration of the measurement
 * @param energy_mj - energy used in microjoules
 */
void logMeasure(const char app[], long long  dauer, long long energy_mj);

/**
 * Function to log measurement result
 * @param res - measurement result to log
 */
void logMeasure(MeasureResult res);

// Function to log a new line in the measurement log file
void logMeasureNewLine();

// Function to flush the measurement log file
void logMeasureFlush();

/**
 * Function to run a command
 * @param command - command to run
 */
void runCommand(const char* command);

/**
 * Function to run and measure a script
 * @param script - script to run and measure
 * @return MeasureResult - result of the measurement
 */
MeasureResult runAndMeasureScript(const char* script);

/**
 * Function to run and measure scripts from a directory
 * @param count - number of scripts to run
 * @param directory - directory containing the scripts
 * @param cores - number of cores
 */
void runAndMeasureScriptsFromDirectory(int count, const char* directory, const char* cores);

/**
 * Function to get filename with parameters
 * @param filename - original filename
 * @param cores - number of cores
 * @return std::string - filename with parameters
 */
std::string  getFilenameWithParam(char* filename, std::string cores);

/**
 * Function to measure application task
 * @param apptaskname - name of the application task
 * @param cpufreq - CPU frequency
 * @param cores - number of cores
 * @return MeasureResult - result of the measurement
 */
MeasureResult measureAppTask(std::string apptaskname, std::string cpufreq, std::string cores);

// Function to select CPU frequency
void selectCpuFrequency();

/**
 * Function to measure all prototype tasks
 * @param count - number of tasks to measure
 */
void measureAllPrototypTasks(int count);

#endif //INC_3_TASKTYPE_ESTIMATOR_MEASURE_H
