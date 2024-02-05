
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

/*int main(int argc, char *argv[]) {
    readFilenamesFromDirectory("gen");
}*/



