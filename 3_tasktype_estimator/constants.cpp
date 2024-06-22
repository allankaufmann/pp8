
#include "include/constants.h"
const char* config_filename = "experiment.config";
const char* foldername_generated_scripts = "gen";
std::string foldername_generated_scripts_tasktypes = "gen/tasktypes";
std::string foldername_generated_scripts_tasktypes_onetomany = "gen/tasktypes/onetomany";
std::string foldername_generated_scripts_apptasks = "gen/apptasks";
std::string foldername_generated_scripts_tasktypes_from_folder = "./" + foldername_generated_scripts_tasktypes;
const char* foldername_seq = "taskmapper/taskseq";
const char* foldername_appseq = "taskmapper/appseq";
std::string filename_taskmap_result = "./results/taskmap.txt";
std::string filename_taskmap_result_from_folder = "./" + filename_taskmap_result;
std::string folder_measure = "../edgedetection/measure";
std::string filename_epebench_ebmodels = "../epEBench/bin/Release/ebmodels.ini";
std::string result_section_one2one="OneToOne";
std::string result_section_log="Log";
std::string result_section_one2many="OneToMany";
std::string result_section_model="model";
std::string result_section_end_model="end_model";
std::string result_section_end_model_space="end_model ";
const char* logfolder_taskmapper = "logs/taskmapper/";
const char* script_test_Edgedetection = "./scripts/testEdgedetection.sh";
const char* script_test_epEBench = "./scripts/testepEBench.sh";
const char* folder_result = "././results/";
const char* logfileext = ".log";
const char* logfileres = ".result";
const int measure_repeats = 5;