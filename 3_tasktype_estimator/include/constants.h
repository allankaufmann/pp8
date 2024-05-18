//
// Created by allan on 28.01.24.
//


#ifndef EXPERIMENT_CONSTANTS_H
#define EXPERIMENT_CONSTANTS_H
#include <string>
extern const char* config_filename;
extern const char* foldername_generated_scripts;
extern std::string foldername_generated_scripts_tasktypes;
extern std::string foldername_generated_scripts_tasktypes_onetomany;
extern std::string foldername_generated_scripts_apptasks;
extern std::string foldername_generated_scripts_apptasks_from_folder;
extern std::string foldername_generated_scripts_tasktypes_from_folder;
extern const char* foldername_seq;
extern const char* foldername_appseq;
extern const char* foldername_edgedetction_scripts;
extern std::string filename_taskmap_result;
extern std::string filename_taskmap_result_from_folder;

extern std::string filename_estimation_result;
extern std::string filename_estimation_result_folder;

extern std::string folder_measure;

extern std::string filename_epebench_ebmodels;
extern std::string result_section_one2one;
extern std::string result_section_log;
extern std::string result_section_one2many;
extern std::string result_section_model;
extern std::string result_section_end_model;
extern std::string result_section_end_model_space;
extern const char* logfolder_taskmapper;
extern const char* script_test_Edgedetection;
extern const char* script_test_epEBench;
extern const char* folder_result;
extern const char* logfileext;
extern const char* logfileres;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#endif //EXPERIMENT_CONSTANTS_H
