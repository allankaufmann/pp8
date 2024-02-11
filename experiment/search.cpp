#include <iostream>
#include <string.h> //strtok
#include <vector>
#include "tools.hpp"
#include "constants.h"
#include <dirent.h>
#include <sys/stat.h> //mkdir
#include <fstream>
#include <iostream>
#include <map>

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

class Result {
    public:
        std::map<std::string, std::map<std::string, int>> resultMap;

};

Result result;

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
}

void closeLogFileSearch() {
    logfileSearch.close();
}


void logSearch(std::string taskname, std::string ptname, int size, int sizeAppTask, int sizeProtTask, int div) {
    logfileSearch << "In AppTask " << taskname << "(Gesamt: " << sizeAppTask << ") sind aus ProtTask " << ptname << " (Gesamt: " << sizeProtTask << ")  " << size << " Einträge vorhanden! (Der Prottask wurde aufgrund der Größe des AppTask " << div << " mal wiederholt!)\n";
    logfileSearch.flush();
}


/**
 * Überprüfung, ob Assemblerbefehl aus Prototyptask im Assemblercode der Anwendungstask vorkommt. Dabei werden bereits verwendete Codestellen übersprungen.
 *
 * @param protTaskSequenceEntry der gesuchte Assemblercode aus dem Prototyptask
 * @param appTask Taskobjekt des Anwendungstasks.
 * @return Anwendungstask nach Ende der Suche.
 */
PrototypTask compareProtTaskSequenEntryWithAppTaskEntry(std::string protTaskSequenceEntry, PrototypTask appTask) {
    for (int i=0; i < appTask.sequenzen.size(); i++) {
        if (appTask.found[i] == true) {
            continue; // Sequenz bereits gefunden, nächster Treffer!
        }

        if (appTask.sequenzen[i].compare(protTaskSequenceEntry) == 0) {
            appTask.found[i]=true; // Sequenz gefunden, wird auf true gesetzt!
            //std::cout << "Sequenzeintrag " << protTaskSequenceEntry << " gefunden! (" << i << ". Position in appTask)\n";
            return appTask;
        }
    }
    return appTask;
}



PrototypTask compareAppTaskWithPrototypTasks(PrototypTask appTask, PrototypTask protTypTask ) {



    /*for (std::string protTaskSequenceEntry : protTypTask.sequenzen) {
        appTask = compareProtTaskSequenEntryWithAppTaskEntry(protTaskSequenceEntry, appTask);
    }*/

    float sizeAppDivProt = (float) appTask.sequenzen.size() / protTypTask.sequenzen.size();
    if (sizeAppDivProt > 0.5) {
        for (int i = 0; i< sizeAppDivProt; i++) {
            std::cout << "(Durchgang " << i << " von " << sizeAppDivProt << ")\n";
            for (std::string protTaskSequenceEntry : protTypTask.sequenzen) {
                //std::cout << "Suche Sequenzeintrag " << protTaskSequenceEntry << " (Durchgang " << i << " von " << sizeAppDivProt << ")\n";
                appTask = compareProtTaskSequenEntryWithAppTaskEntry(protTaskSequenceEntry, appTask);
            }
        }

    }

    int anzahl_hits = 0;
    for (bool found : appTask.found) {
        if (found) {
            anzahl_hits++;
        }
    }
    logSearch(appTask.name, protTypTask.name, anzahl_hits, appTask.sequenzen.size(), protTypTask.sequenzen.size(), sizeAppDivProt );

    if (result.resultMap[appTask.name][protTypTask.name]==0 || result.resultMap[appTask.name][protTypTask.name] < anzahl_hits) {
        result.resultMap[appTask.name][protTypTask.name]=anzahl_hits;
    }

    appTask.resetFound();
    return appTask;
}

void analyseAppTask(PrototypTask appTask) {
    std::cout << "\nAppTask " << appTask.name << " wird geprüft!\n";

    for (PrototypTask protTypTask : prottaskVektor) {
        std::cout << "Vergleich mit protTypTask " << protTypTask.name << "\n";
        appTask = compareAppTaskWithPrototypTasks(appTask, protTypTask);
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

void logBestTask(PrototypTask t) {
    std::string bestName;
    int besthit = 0;
    std::map<std::string, int>::iterator it;
    for (it = result.resultMap[t.name].begin(); it != result.resultMap[t.name].end(); it++) {
        if (it->second>besthit) {
            besthit=it->second;
            bestName=it->first;
        }

    }

    logfileSearch << "Der ähnlichste ProttypTask für den AppTask " << t.name << " ist " << bestName << "(" << besthit << " Treffer)\n\n";
}

void test() {
    initProttaskVektor();
    initAppTaskVektor();

    openLogfileSearch();

    int count = 0; // test
    for (PrototypTask t : apptaskVektor) {
        analyseAppTask(t);
        logfileSearch << "\n";

        logBestTask(t);

        count++;
        if (count==2) {
            //break; // zu Testzwecken
        }

    }
    closeLogFileSearch();
}