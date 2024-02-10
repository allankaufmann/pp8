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

        void resetFound() {
            found.clear();
            for (std::string s : sequenzen) {
                found.push_back(false);
            }
        }

};

PrototypTask currentProtTypTask;
PrototypTask currentAppTask;

std::vector<PrototypTask> prottaskVektor;
std::vector<PrototypTask> apptaskVektor;

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
        t.found.push_back(false);
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


void logSearch(std::string taskname, std::string ptname, int size, int sizeAppTask, int sizeProtTask) {
    //std::cout << "In AppTask " << t.name << " sind aus ProtTask " << pt.name << " " << pt.found.size() << " Einträge vorhanden!\n";
    //std::fprintf(logfileSearch, "%s", taskname);
    logfileSearch << "In AppTask " << taskname << "(" << sizeAppTask << ") sind aus ProtTask " << ptname << " (" << sizeProtTask << ")  " << size << " Einträge vorhanden!\n";
    logfileSearch.flush();
    //fprintf(logfileSearch, "%s", taskname);
}


PrototypTask compareProtTaskSequenEntryWithAppTaskEntry(std::string protTaskSequenceEntry, PrototypTask appTask) {
    for (int i=0; i < currentAppTask.sequenzen.size(); i++) {
        if (appTask.found[i] == true) {
            continue; // Sequenz bereits gefunden, nächster Treffer!
        }

        if (appTask.sequenzen[i].compare(protTaskSequenceEntry) == 0) {
            appTask.found[i]=true; // Sequenz gefunden, wird auf true gesetzt!
            return appTask;
        }
    }
    return appTask;
}



void compareAppTaskWithPrototypTasks(PrototypTask appTask) {
    for (std::string protTaskSequenceEntry : currentProtTypTask.sequenzen) {
        currentAppTask = compareProtTaskSequenEntryWithAppTaskEntry(protTaskSequenceEntry, currentAppTask);


        /*for (std::string appTaskSequenceEntry : appTask.sequenzen) {
            appTaskSequenceEntry.
            if (protTaskSequenceEntry.compare(appTaskSequenceEntry) == 0) {
                protTypTask.found.push_back(true);
                break;
            }
        }*/
    }

    int anzahl_hits = 0;
    for (bool found : currentAppTask.found) {
        if (found) {
            anzahl_hits++;
        }
    }

    //std::cout << "In AppTask " << t.name << " sind aus ProtTask " << pt.name << " " << pt.found.size() << " Einträge vorhanden!\n";
    logSearch(appTask.name, currentProtTypTask.name, anzahl_hits, currentAppTask.sequenzen.size(), currentProtTypTask.sequenzen.size() );
    currentAppTask.resetFound();

}

void analyseAppTask(PrototypTask appTask) {
    std::cout << "\nAppTask " << appTask.name << " wird geprüft!\n";

    for (PrototypTask protTypTask : prottaskVektor) {
        currentProtTypTask = protTypTask;
        compareAppTaskWithPrototypTasks(currentAppTask);
    }
}

void initProttaskVektor() {
    std::vector<char*> taskseqnames = readFilenamesFromDirectory(foldername_seq);
    for (char* seqname : taskseqnames) {
        PrototypTask t = readSeqFile(seqname);
        prottaskVektor.push_back(t);
        printf("Der Task %s enthält %d Einträge\n", seqname, t.sequenzen.size());
    }
    printf("%s", "\n");
}

void initAppTaskVektor() {
    std::vector<char*> appseqnames = readFilenamesFromDirectory(foldername_appseq);
    for (char* seqname : appseqnames) {
        PrototypTask t = readSeqFile(seqname);
        apptaskVektor.push_back(t);
        printf("Der Task %s enthält %d Einträge\n", seqname, t.sequenzen.size());
    }
    printf("%s", "\n");
}

void test() {
    initProttaskVektor();
    initAppTaskVektor();

    //std::vector<PrototypTask>::iterator it = apptaskVektor.begin();
    openLogfileSearch();
    for (PrototypTask t : apptaskVektor) {
        currentAppTask=t;
        analyseAppTask(t);
        logfileSearch << "\n";
    }
    closeLogFileSearch();


    //PrototypTask apptask1 = apptaskVektor.



    //readSeqFile(daddseq);
    //readSeqFile(sampleappseq);

}