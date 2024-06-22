
#ifndef INC_3_TASKTYPE_ESTIMATOR_CONFIG_H
#define INC_3_TASKTYPE_ESTIMATOR_CONFIG_H
#include "vector"
#include <string.h>
#include "string"

// Vector to store CPU frequencies
extern std::vector<std::string> cpuFrequencyVektor;

// Vector to store parallelism options
extern std::vector<std::string> parallelismVektor;

// Vector to store task types
extern std::vector<std::string> tasktypeVektor;

// Vector to store application types
extern std::vector<std::string> apptypeVektor;

// Section name for task types in the configuration file
extern std::string section_tasktypes;

// Section name for application tasks in the configuration file
extern std::string section_apptasks;

// Section name for CPU frequency in the configuration file
extern std::string section_cpufrequency;

// Section name for parallelism in the configuration file
extern std::string section_parallelism;

[[deprecated("Diese Funktion ist veraltet und wird möglicherweise in zukünftigen Versionen entfernt.")]]
long long read_power_usage();

/**
 * Function to read the configuration file
 * @param showEntries - boolean to decide whether to show entries or not
 * @param generateScripts - boolean to decide whether to generate scripts or not
 */
void readConfigFile(bool showEntries, bool generateScripts);

/**
 * Function to generate scripts for task types
 * @param folder - the folder to generate the scripts in
 * @param task - the task for which to generate the scripts
 */
void generateTaskTypeScripts(std::string folder, std::string task);

/**
 * Function to generate scripts for application tasks
 * @param task - the task for which to generate the scripts
 */
void generateAppTaskScripts(std::string task);

/**
 * Function to read energy in microjoules
 * @return energy in microjoules as long unsigned
 */
long unsigned  readEnergy_UJ();

/**
 * Function to safely read energy in microjoules. Will loop until the energy counter changed.
 * @return energy in microjoules as long unsigned
 */
long unsigned  readEnergy_UJ_secure();

/**
 * Function to check preconditions on this system for estimation. Will give feedback if something is wrong.
 */
void checkPreconditions();
#endif //INC_3_TASKTYPE_ESTIMATOR_CONFIG_H
