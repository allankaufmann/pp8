//
// Created by allan on 12.05.24.
//

#ifndef INC_3_TASKTYPE_ESTIMATOR_CONFIG_H
#define INC_3_TASKTYPE_ESTIMATOR_CONFIG_H
long long read_power_usage();
void readConfigFile(bool showEntries, bool generateScripts);
extern std::vector<std::string> cpuFrequencyVektor;
extern std::vector<std::string> parallelismVektor;
extern std::vector<std::string> tasktypeVektor;
extern std::vector<std::string> apptypeVektor;
#endif //INC_3_TASKTYPE_ESTIMATOR_CONFIG_H
