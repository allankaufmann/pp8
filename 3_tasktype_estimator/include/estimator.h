
#ifndef INC_3_TASKTYPE_ESTIMATOR_ESTIMATOR_H
#define INC_3_TASKTYPE_ESTIMATOR_ESTIMATOR_H
#include <sys/stat.h> //mkdir, chmod
#include <fstream>
#include <string>
#include "../include/constants.h"
#include "../include/tools.hpp"
#include "../include/MeasureResult.h"
#include "../include/measure.h"
#include "../include/config.h"

// Log file for results
extern std::ofstream resultFile;

/**
 * Function to open the result file
 */
void openResultFile();

/**
 * Function to close the result file
 */
void closeResultFile();

/**
 * Function to log the headline
 */
void logHeadline();

/**
 * Function to log an empty line
 */
void logEmptyline();

/**
 * Function to start application task estimation
 * @param apptaskname - name of the application task
 * @param repeat - number of times to repeat the estimation
 */
void startApptaskEstimation(std::string apptaskname, int repeat);

/**
 * Function to repeat estimations for an application task
 * @param apptaskname - name of the application task
 * @param repeats - number of times to repeat the estimation
 */
void repeatEstimationsForAppTask(std::string apptaskname, int repeats);

/**
 * Function to start the estimation
 * @param repeats - number of times to repeat the estimation
 */
void startEstimation(int repeats);

/**
 * Function to test the estimation
 * @param apptaskname - name of the application task
 * @param repeats - number of times to repeat the test
 */
void testEstimation(std::string apptaskname, int repeats);

#endif //INC_3_TASKTYPE_ESTIMATOR_ESTIMATOR_H
