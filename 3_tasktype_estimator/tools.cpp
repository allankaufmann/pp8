

#include <unistd.h>
#include <fstream>
#include "tools.hpp"

#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h> //mkdir
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>




std::vector<char*> readFilenamesFromDirectory(const char* directoryname) {
    std::vector<char*> v;
    DIR *dir = opendir(directoryname);
    if(dir!=NULL) {
        struct dirent *dent;
        while((dent=readdir(dir))!=NULL) {
            char* file = dent->d_name;
            if (strcmp(file, ".") == 0 || strcmp(file, "..") == 0) {
                continue;
            }
            char* filename = (char*) malloc(sizeof(char) * (strlen(file) + strlen(directoryname) + 10));
            sprintf(filename, "%s/%s", directoryname, file);
            v.push_back(filename);
        }
        //printf("%lu", v.size());
    }
    return v;
};

char* getFilename(const char *const folder) {
    time_t mytime = time(NULL);
    tm local_tm = *localtime(&mytime);

    int year = local_tm.tm_year + 1900;
    int month = local_tm.tm_mon+1;

    char* filename = (char*) malloc(sizeof(char) * 30);
    sprintf(filename, "%s%d%s%d%s%d%s%d%s%d.log",
            folder,
            year,
            (month<10) ? "0" : "",
            month,
            (local_tm.tm_mday<10) ? "0" : "",
            local_tm.tm_mday,
            (local_tm.tm_hour<10) ? "0" : "",
            local_tm.tm_hour,
            (local_tm.tm_min<10) ? "0" : "",
            local_tm.tm_min);

    return filename;
}

/*int main(int argc, char *argv[]) {
    readFilenamesFromDirectory("gen");
}*/



void extractAppTaskNameFromFileName(const char* filename, char* taskname) {
    const char *start = strrchr(filename, '/');
    const char *end = strchr(filename, '.');

    if (start != NULL && end != NULL && end > start) {
        size_t length = end - start - 1; //


        strncpy(taskname, start + 1, length);
        taskname[length] = '\0'; // Null-terminate the string
    } else {
        taskname[0] = '\0'; // If no '/' and '.', return an empty string
    }
}

void extractTaskNameFromFileName(const char* filename, char* taskname) {
    const char *start = strrchr(filename, '/');
    const char *end = strrchr(filename, '.');

    if (start != NULL && end != NULL && end > start) {
        size_t length = end - start - 1; //


        strncpy(taskname, start + 1, length);
        taskname[length] = '\0'; // Null-terminate the string
    } else {
        taskname[0] = '\0'; // If no '/' and '.', return an empty string
    }
}

char* searchTasktypeFile(std::string tasktypename, std::string folder) {
    std::vector<char*> v_filenames = readFilenamesFromDirectory(folder.c_str());

    for (char* filename : v_filenames) {
        char c_taskname[strlen(filename)];
        extractTaskNameFromFileName(filename, c_taskname);
        if (strcmp(c_taskname, tasktypename.c_str())==0) {
            return filename;
        }
    }
    return NULL;
}