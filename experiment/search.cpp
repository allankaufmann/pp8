#include <iostream>
#include <string.h> //strtok
#include <vector>
#include "tools.hpp"
#include "constants.h"
#include <dirent.h>
#include <sys/stat.h> //mkdir
#include <fstream>
#include <iostream>
const char* daddseq= "search/taskseq/dadd.seq";
const char* sampleappseq = "search/appseq/sampleapp.seq";

std::ofstream logfileSearch;
//FILE* logfileSearch;
static const char *const logfolder_search = "logs/search/";

class PrototypTask {
    public:
        std::string name;
        std::vector<std::string> sequenzen;
        std::vector<bool> found;
};

PrototypTask readSeqFile(const char* file) {
    PrototypTask t;
    t.name=file;


    FILE* filePointer;
    filePointer = fopen(file, "r");

    int bufferLength = 255;
    char buffer[bufferLength];

    char* task;

    while(fgets(buffer, bufferLength, filePointer)) {
        task = strtok(buffer, " ");
        //printf("%s\n", task);
        t.sequenzen.push_back(task);
    }

    fclose(filePointer);
    return t;
}

void openLogfileSearch() {
    mkdir(logfolder_search, 0777);
    logfileSearch.open(getFilename(logfolder_search));
    logfileSearch << "x;y;z\n";
    //logfileSearch = fopen(getFilename(logfolder_search), "w");
    //fprintf(logfileSearch, "x;y;z;\n");
}

void closeLogFileSearch() {
    logfileSearch.close();
}

void logSearch(std::string taskname, std::string ptname, int size) {
    //std::cout << "In AppTask " << t.name << " sind aus ProtTask " << pt.name << " " << pt.found.size() << " Einträge vorhanden!\n";
    //std::fprintf(logfileSearch, "%s", taskname);
    logfileSearch << "In AppTask " << taskname << " sind aus ProtTask " << ptname << " " << size << " Einträge vorhanden!\n";
    //fprintf(logfileSearch, "%s", taskname);
}


void test() {
    std::vector<char*> taskseqnames = readFilenamesFromDirectory(foldername_seq);

    std::vector<PrototypTask> prottaskVektor;
    std::vector<PrototypTask> apptaskVektor;

    for (char* seqname : taskseqnames) {
        PrototypTask t = readSeqFile(seqname);
        prottaskVektor.push_back(t);
        printf("Der Task %s enthält %d Einträge\n", seqname, t.sequenzen.size());
    }
    printf("%s", "\n");
    std::vector<char*> appseqnames = readFilenamesFromDirectory(foldername_appseq);
    for (char* seqname : appseqnames) {
        PrototypTask t = readSeqFile(seqname);
        apptaskVektor.push_back(t);
        printf("Der Task %s enthält %d Einträge\n", seqname, t.sequenzen.size());
    }
    printf("%s", "\n");
    //printf("%d", apptaskVektor.size());

    //std::vector<PrototypTask>::iterator it = apptaskVektor.begin();
    openLogfileSearch();
    for (PrototypTask t : apptaskVektor) {
        std::cout << "\nAppTask " << t.name << " wird geprüft!\n";

        for (PrototypTask pt : prottaskVektor) {

            for (std::string pts : pt.sequenzen) {
                for (std::string ts : t.sequenzen) {

                    if (pts.compare(ts)==0) {
                        pt.found.push_back(true);
                        break;
                    }
                }
            }
            //std::cout << "In AppTask " << t.name << " sind aus ProtTask " << pt.name << " " << pt.found.size() << " Einträge vorhanden!\n";
            logSearch(t.name, pt.name, pt.found.size() );
            pt.found.clear();
        }

        //printf("AppTask %s wird geprüft!", t.name);

    }
    closeLogFileSearch();


    //PrototypTask apptask1 = apptaskVektor.



    //readSeqFile(daddseq);
    //readSeqFile(sampleappseq);

}