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


/**
 * Startet die eigentliche Ähnlichkeitssuche zwischen einem Anwendungstask und einem prototypischen Task.
 *
 *
 * @param appTask zu untersuchender Anwendungstask
 * @param protTypTask zu vergleichender prototypischer Task
 * @return liefert den Anwendungstask nach Untersuchung zurück
 */
PrototypTask compareAppTaskWithPrototypTasks(PrototypTask appTask, PrototypTask protTypTask ) {

    /*
     * Die Anzahl der Instruktionen des Anwendungstask kann deutlich höher sein, als die Anzahl aus dem Prototyptask. Um die Ähnlichkeit zu überprüfen ist es daher sinnvoller, wenn
     * der Prototyptask erneut so oft wiederholt wird, dass diese der Anzahl aus dem Anwendungstask annäherend entspricht.
     */
    float sizeAppDivProt = (float) appTask.sequenzen.size() / protTypTask.sequenzen.size();

    /*
     * Die Anzahl der Instruktionen des Anwendungstask sollte nicht niedriger sein, als die Anzahl des Prototyptasks. Wenn 50% nicht erreicht sind, dann wird dieser Vergleich nicht durchgeführt.
     */
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

/**
 * Für den übergebenen Anwendungstask wird eine Ähnlichkeitssuche ausgeführt. Dabei werden nacheinander mit den vorhandenen Prototyptasks vergleichen.
 *
 * @param appTask zu untersuchender Anwendungstask.
 */
void analyseAppTask(PrototypTask appTask) {
    std::cout << "\nAppTask " << appTask.name << " wird geprüft!\n";

    for (PrototypTask protTypTask : prottaskVektor) {
        std::cout << "Vergleich mit protTypTask " << protTypTask.name << "\n";
        appTask = compareAppTaskWithPrototypTasks(appTask, protTypTask);
    }
}

/**
 * Liest die Sequenzdateien der prototypischen Tasks aus und bereitet einen Vektor für die Ähnlichkeitssuche vor.
 */
void initProttaskVektor() {
    std::vector<char*> taskseqnames = readFilenamesFromDirectory(foldername_seq);
    for (char* seqname : taskseqnames) {
        PrototypTask t = readSeqFile(seqname);
        prottaskVektor.push_back(t);
        printf("Der Task %s enthält %d Einträge\n", seqname, t.sequenzen.size());
    }
    printf("%s", "\n");
}

/**
 * Liest die Sequenzdateien der Anwendungstasks aus und bereitet einen Vektor für die Ähnlichkeitssuche vor.
 */
void initAppTaskVektor() {
    std::vector<char*> appseqnames = readFilenamesFromDirectory(foldername_appseq);
    for (char* seqname : appseqnames) {
        PrototypTask t = readSeqFile(seqname);
        apptaskVektor.push_back(t);
        printf("Der Task %s enthält %d Einträge\n", seqname, t.sequenzen.size());
    }
    printf("%s", "\n");
}

/**
 * Wird nach Ausführung der Ähnlichkeitssuche eines Anwendungstasks ausgeführt. Zu dem übergebenen Anwendungstask soll der Prototyptask ausgegeben werden, der dem Anwendungstask am ähnlichsten ist.
 *
 * @param appTask für diesen Anwendungstask wird das Ergebnis geprüft.
 */
void logBestTask(PrototypTask appTask) {
    std::string bestName;
    int besthit = 0;
    std::map<std::string, int>::iterator it;
    for (it = result.resultMap[appTask.name].begin(); it != result.resultMap[appTask.name].end(); it++) {
        if (it->second>besthit) {
            besthit=it->second;
            bestName=it->first;
        }
    }
    logfileSearch << "Der ähnlichste ProttypTask für den AppTask " << appTask.name << " ist " << bestName << "(" << besthit << " Treffer)\n\n";
}

bool checkSequenzfiles() {
    initProttaskVektor();

    if (prottaskVektor.empty()) {
        printf("Der Ordner %s enthält keine Sequenzen (Anwendung prototyptaskscpp ausgeführt?)", foldername_seq);
        return false;
    }

    initAppTaskVektor();
    if (apptaskVektor.empty()) {
        printf("Der Ordner %s enthält keine Sequenzen! (Anwendung apptasks ausgeführt?)", foldername_appseq);
        return false;
    }
    return true;
}

void compareAppTaskProtTasksOneToOne() {
    if (checkSequenzfiles()==false) {
        return;
    }

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

PrototypTask compareAppTaskWithPrototypTasksMany(PrototypTask appTask, PrototypTask protTypTask ) {
    for (std::string protTaskSequenceEntry : protTypTask.sequenzen) {
        //std::cout << "Suche Sequenzeintrag " << protTaskSequenceEntry << " (Durchgang " << i << " von " << sizeAppDivProt << ")\n";
        appTask = compareProtTaskSequenEntryWithAppTaskEntry(protTaskSequenceEntry, appTask);
    }

    int anzahl_hits = 0;
    for (bool found : appTask.found) {
        if (found) {
            anzahl_hits++;
        }
    }
    logSearch(appTask.name, protTypTask.name, anzahl_hits, appTask.sequenzen.size(), protTypTask.sequenzen.size(), 1 );

    if (result.resultMap[appTask.name][protTypTask.name]==0 || result.resultMap[appTask.name][protTypTask.name] < anzahl_hits) {
        result.resultMap[appTask.name][protTypTask.name]=anzahl_hits;
    }

    appTask.resetFound();
    return appTask;
}

/**
 * Für den übergebenen Anwendungstask wird eine Ähnlichkeitssuche ausgeführt. Dabei werden nacheinander mit den vorhandenen Prototyptasks vergleichen.
 *
 * @param appTask zu untersuchender Anwendungstask.
 */
void analyseAppTaskMany(PrototypTask appTask) {
    std::cout << "\nAppTask " << appTask.name << " wird geprüft!\n";

    for (PrototypTask protTypTask : prottaskVektor) {
        std::cout << "Vergleich mit protTypTask " << protTypTask.name << "\n";
        appTask = compareAppTaskWithPrototypTasksMany(appTask, protTypTask);
    }
}

void compareAppTaskProtTasksOneToMany() {
    if (checkSequenzfiles()==false) {
        return;
    }
    openLogfileSearch();
    PrototypTask task = apptaskVektor[0];
    analyseAppTaskMany(task);

    logBestTask(task);
    closeLogFileSearch();
}