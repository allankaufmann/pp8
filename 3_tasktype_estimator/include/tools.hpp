#include <string>
#ifndef EXPERIMENT_TOOLS_HPP
#define EXPERIMENT_TOOLS_HPP

#include <vector>

std::vector<char*> readFilenamesFromDirectory(const char* directoryname);
char* getFilename(const char *const folder, const char* ext);
char* searchTasktypeFile(std::string tasktypename, std::string folder);
void extractTaskNameFromFileName(const char* filename, char* taskname);
void extractAppTaskNameFromFileName(const char* filename, char* taskname);
std::string readOneToOneMapping(std::string apptaskname);
#endif //EXPERIMENT_TOOLS_HPP
