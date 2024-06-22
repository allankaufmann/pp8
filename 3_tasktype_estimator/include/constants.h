

#ifndef EXPERIMENT_CONSTANTS_H
#define EXPERIMENT_CONSTANTS_H
#include <string>

// Name of the configuration file
extern const char* config_filename;

// Name of the folder for generated scripts
extern const char* foldername_generated_scripts;

// Name of the folder for generated scripts for task types
extern std::string foldername_generated_scripts_tasktypes;

// Name of the folder for generated scripts for task types (one to many)
extern std::string foldername_generated_scripts_tasktypes_onetomany;

// Name of the folder for generated scripts for application tasks
extern std::string foldername_generated_scripts_apptasks;

// Name of the folder for generated scripts for task types from a folder
extern std::string foldername_generated_scripts_tasktypes_from_folder;

// Name of the folder for sequential tasks
extern const char* foldername_seq;

// Name of the folder for application sequential tasks
extern const char* foldername_appseq;

// Name of the result file for task map
extern std::string filename_taskmap_result;

// Name of the result file for task map from a folder
extern std::string filename_taskmap_result_from_folder;

// Name of the folder for measurements
extern std::string folder_measure;

// Name of the file for epebench ebmodels
extern std::string filename_epebench_ebmodels;

// Section name for one to one result
extern std::string result_section_one2one;

// Section name for log result
extern std::string result_section_log;

// Section name for one to many result
extern std::string result_section_one2many;

// Section name for model result
extern std::string result_section_model;

// Section name for end model result
extern std::string result_section_end_model;

// Section name for end model result with space
extern std::string result_section_end_model_space;

// Name of the folder for task mapper logs
extern const char* logfolder_taskmapper;

// Name of the test script for Edge detection
extern const char* script_test_Edgedetection;

// Name of the test script for epEBench
extern const char* script_test_epEBench;

// Name of the folder for results
extern const char* folder_result;

// Extension for log files
extern const char* logfileext;

// Name of the result log file
extern const char* logfileres;

// Number of times to repeat measurements
extern const int measure_repeats;

// Reset color for console output
#define RESET   "\033[0m"

// Red color for console output
#define RED     "\033[31m"


#endif //EXPERIMENT_CONSTANTS_H
