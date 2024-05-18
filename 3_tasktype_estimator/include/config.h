
#ifndef INC_3_TASKTYPE_ESTIMATOR_CONFIG_H
#define INC_3_TASKTYPE_ESTIMATOR_CONFIG_H
#include "vector"
#include <string.h>
#include "string"
extern std::vector<std::string> cpuFrequencyVektor;
extern std::vector<std::string> parallelismVektor;
extern std::vector<std::string> tasktypeVektor;
extern std::vector<std::string> apptypeVektor;
extern std::string section_tasktypes;
extern std::string section_apptasks;
extern std::string section_cpufrequency;
extern std::string section_parallelism;

long long read_power_usage();
void readConfigFile(bool showEntries, bool generateScripts);
void generateTaskTypeScripts(std::string folder, std::string task);
void generateAppTaskScripts(std::string task);
long unsigned  readEnergy_UJ();
void checkPreconditions();
#endif //INC_3_TASKTYPE_ESTIMATOR_CONFIG_H
