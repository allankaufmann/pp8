#include <string>
#ifndef EXPERIMENT_TOOLS_HPP
#define EXPERIMENT_TOOLS_HPP

#include <vector>

/**
 * Function to read filenames from a directory
 * @param directoryname - Name of the directory to read from
 * @return std::vector<char*> - Vector of filenames read from the directory
 */
std::vector<char*> readFilenamesFromDirectory(const char* directoryname);

/**
 * Function to get the filename from a folder with a specific extension
 * @param folder - Name of the folder to search in
 * @param ext - Extension of the file to search for
 * @return char* - Filename found in the folder with the specified extension
 */
char* getFilename(const char *const folder, const char* ext);

/**
 * Function to search for a task type file in a specific folder
 * @param tasktypename - Name of the task type to search for
 * @param folder - Name of the folder to search in
 * @return char* - Task type file found in the folder
 */
char* searchTasktypeFile(std::string tasktypename, std::string folder);

/**
 * Function to extract the task name from a filename
 * @param filename - Name of the file to extract the task name from
 * @param taskname - Pointer to store the extracted task name
 */
void extractTaskNameFromFileName(const char* filename, char* taskname);

/**
 * Function to extract the application task name from a filename
 * @param filename - Name of the file to extract the application task name from
 * @param taskname - Pointer to store the extracted application task name
 */
void extractAppTaskNameFromFileName(const char* filename, char* taskname);

/**
 * Function to read one-to-one mapping from an application task name
 * @param apptaskname - Name of the application task to read the mapping from
 * @return std::string - One-to-one mapping read from the application task name
 */
std::string readOneToOneMapping(std::string apptaskname);

#endif //EXPERIMENT_TOOLS_HPP
