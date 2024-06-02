
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
extern std::ofstream resultFile;
void openResultFile();
void closeResultFile();
std::string readOneToOneMapping(std::string apptaskname);
void logHeadline();
void logEmptyline();
void startApptaskEstimation(std::string apptaskname);
void repeateEstimationsForAppTask(std::string apptaskname, int repeats);
void startEstimation(int repeats);
void testEstimation(std::string apptaskname, int repeats);

#endif //INC_3_TASKTYPE_ESTIMATOR_ESTIMATOR_H
