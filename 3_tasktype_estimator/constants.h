//
// Created by allan on 28.01.24.
//
#include <string>

#ifndef EXPERIMENT_CONSTANTS_H
#define EXPERIMENT_CONSTANTS_H
const char* config_filename = "experiment.config";
const char* foldername_generated_scripts = "gen";
std::string foldername_generated_scripts_tasktypes = "gen/tasktypes";
std::string foldername_generated_scripts_tasktypes_onetomany = "gen/tasktypes/onetomany";
std::string foldername_generated_scripts_apptasks = "gen/apptasks";
std::string foldername_generated_scripts_apptasks_from_folder = "./" + foldername_generated_scripts_apptasks;
std::string foldername_generated_scripts_tasktypes_from_folder = "./" + foldername_generated_scripts_tasktypes;
const char* foldername_seq = "taskmapper/taskseq";
const char* foldername_appseq = "taskmapper/appseq";
const char* foldername_edgedetction_scripts = "scripts/edgedetection";
std::string filename_taskmap_result = "./results/taskmap.txt";
std::string filename_taskmap_result_from_folder = "./" + filename_taskmap_result;

std::string filename_estimation_result = "results/result.txt";
std::string filename_estimation_result_from_folder = "./" + filename_estimation_result;

std::string folder_measure = "../edgedetection/measure";

std::string filename_epebench_ebmodels = "../epEBench/bin/Release/ebmodels.ini";
std::string result_section_one2one="OneToOne";
std::string result_section_log="Log";
std::string result_section_one2many="OneToMany";
std::string result_section_model="model";
std::string result_section_end_model="end_model";
std::string result_section_end_model_space="end_model ";
static const char *const logfolder_taskmapper = "logs/taskmapper/";
const char* script_Sample_Application = "./scripts/runSampleApplication.sh";

#define RESET   "\033[0m"
#define RED     "\033[31m"
#endif //EXPERIMENT_CONSTANTS_H
