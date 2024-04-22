//
// Created by allan on 28.01.24.
//
#include <string>

#ifndef EXPERIMENT_CONSTANTS_H
#define EXPERIMENT_CONSTANTS_H
const char* config_filename = "experiment.ini";
const char* config_filename2 = "experiment.config";
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
std::string filename_estimation_result = "./results/result.txt";
std::string folder_measure = "../edgedetection/measure";
#endif //EXPERIMENT_CONSTANTS_H
