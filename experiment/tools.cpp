
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



