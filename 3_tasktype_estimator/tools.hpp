//
// Created by allan on 31.01.24.
//

#ifndef EXPERIMENT_TOOLS_HPP
#define EXPERIMENT_TOOLS_HPP

#include <vector>

std::vector<char*> readFilenamesFromDirectory(const char* directoryname);
char* getFilename(const char *const folder, const char* ext);
#endif //EXPERIMENT_TOOLS_HPP
