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
#include <list>

const char* daddseq= "search/taskseq/dadd.seq";
const char* sampleappseq = "search/appseq/sampleapp.seq";



std::ofstream logfileSearch;
//FILE* logfileSearch;
static const char *const logfolder_search = "logs/search/";


class Result {
public:
    std::map<std::string, std::map<std::string, int>> protTaskAnzTrefferMap; // <AnwTask, <Prottask, Treffer>>
    std::map<std::string, std::vector<bool>> pptFoundMapWithBool; // für jeden PPT die Treffer speichern
    std::list<std::string> list; // Namen der abgebildeten PPTs.

    void resetPptFoundMap() {
        pptFoundMapWithBool.clear();
    }

};

class Task {
    public:
        std::string name;
        std::vector<std::string> sequenzen;
};

class PrototypTask : public Task {
    std::vector<bool> hit; // Übereinstimmung bei Vergleich

    void resetHit(Task anwTask) {
        hit.clear();
        for (std::string s : anwTask.sequenzen) {
            hit.push_back(false);
        }
    }
};

class AnwTask : public Task {
public:
    std::vector<bool> found;
    Result resultOneToOne;

    std::vector<bool> getTaskHitList(PrototypTask ptt) {
        return resultOneToOne.pptFoundMapWithBool[ptt.name];
    }

    void initTaskHitList(PrototypTask ptt){
        std::vector<bool> hit;
        for (int i = 0; i < sequenzen.size(); i++) {
            hit.push_back(false);
        }

        resultOneToOne.pptFoundMapWithBool[ptt.name]=hit;
    }

    void mergeFound(std::vector<bool> hit) {
        for (int i = 0; i < sequenzen.size(); i++) {
            if (found[i]) {
                continue;
            }
            if (hit[i]) {
                found[i]=true;
            }
        }
    }


    void resetFound() {
        found.clear();
        for (std::string s : sequenzen) {
            found.push_back(false);
        }
    }

    int countFound() {
        int count = 0;
        for (bool b : found) {
            if (b) {
                count++;
            }
        }
        return count;
    }

    /**
     * Zählt alle nicht gefundenen bis Count erreicht wurde.
     *
     * @param count
     * @return
     */
    int maxIndex(int max) {
        int count = 0;
        for (int i = 0; i < found.size(); i++) {
            if (!found[i]) {
                count++;
            }
            if (count>=max){
                return i;
            }
        }
        return found.size()-1;
    }

    std::string bestName;
    float besthit = 0;

    void calcBestTask() {
        std::map<std::string, int>::iterator it;
        for (it = resultOneToOne.protTaskAnzTrefferMap[name].begin(); it != resultOneToOne.protTaskAnzTrefferMap[name].end(); it++) {
            if (it->second>besthit) {
                besthit=it->second;
                bestName=it->first;
            }
        }
    }



};

std::vector<PrototypTask> prottaskVektor;
std::vector<AnwTask> apptaskVektor;


std::vector<std::string> readSeqFile(const char* file, Task t) {
    std::vector<std::string> sequenzen;

    FILE* filePointer;
    filePointer = fopen(file, "r");

    int bufferLength = 255;
    char buffer[bufferLength];

    char* task;

    while(fgets(buffer, bufferLength, filePointer)) {
        task = strtok(buffer, " ");
        //printf("%s\n", task);
        sequenzen.push_back(task);
    }

    fclose(filePointer);
    return sequenzen;
}

PrototypTask readProttypTaskSeqfile(const char* file) {
    PrototypTask t;
    t.name=file;
    t.sequenzen=readSeqFile(file, t);
    return t;
}

AnwTask readAnwTaskSeqfile(const char* file) {
    AnwTask t;
    t.name=file;
    t.sequenzen=readSeqFile(file, t);
    t.resetFound();
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
AnwTask compareProtTaskSequenEntryWithAppTaskEntry(std::string protTaskSequenceEntry, AnwTask appTask, int maxIndex) {
    for (int i=0; i < appTask.sequenzen.size(); i++) {
        if (appTask.found[i] == true) {
            continue; // Sequenz bereits gefunden, nächster Treffer!
        }

        if (maxIndex!=0 && i > maxIndex) {
            return appTask;
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
 * Überprüfung, ob Assemblerbefehl aus Prototyptask im Assemblercode der Anwendungstask vorkommt. Dabei werden bereits verwendete Codestellen übersprungen.
 *
 * @param protTaskSequenceEntry der gesuchte Assemblercode aus dem Prototyptask
 * @param appTask Taskobjekt des Anwendungstasks.
 * @return Anwendungstask nach Ende der Suche.
 */
AnwTask compareProtTaskSequenEntryWithAppTaskEntryMany(std::string protTaskSequenceEntry, AnwTask appTask, PrototypTask protTypTask, int maxIndex) {
    std::vector<bool> hitList = appTask.getTaskHitList(protTypTask);

    for (int i=0; i < appTask.sequenzen.size(); i++) {
        if (appTask.found[i] || hitList[i] == true) {
            continue; // Sequenz bereits gefunden, nächster Treffer!
        }

        if (maxIndex!=0 && i > maxIndex) {
            return appTask;
        }

        if (appTask.sequenzen[i].compare(protTaskSequenceEntry) == 0) {
            //appTask.getTaskHitList(protTypTask)[i]=true;
            appTask.resultOneToOne.pptFoundMapWithBool[protTypTask.name][i]=true;
            //hitList[i]=true; // Sequenz gefunden, wird auf true gesetzt!
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
AnwTask compareAppTaskWithPrototypTasks(AnwTask appTask, PrototypTask protTypTask ) {

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
                appTask = compareProtTaskSequenEntryWithAppTaskEntry(protTaskSequenceEntry, appTask, 0);
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

    if (appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.name][protTypTask.name] == 0 || appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.name][protTypTask.name] < anzahl_hits) {
        appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.name][protTypTask.name]=anzahl_hits;
    }

    appTask.resetFound();
    return appTask;
}

/**
 * Für den übergebenen Anwendungstask wird eine Ähnlichkeitssuche ausgeführt. Dabei werden nacheinander mit den vorhandenen Prototyptasks vergleichen.
 *
 * @param appTask zu untersuchender Anwendungstask.
 */
AnwTask analyseAppTask(AnwTask appTask) {
    std::cout << "\nAppTask " << appTask.name << " wird geprüft!\n";

    for (PrototypTask protTypTask : prottaskVektor) {
        std::cout << "Vergleich mit protTypTask " << protTypTask.name << "\n";
        appTask = compareAppTaskWithPrototypTasks(appTask, protTypTask);
    }
    return appTask;
}

/**
 * Liest die Sequenzdateien der prototypischen Tasks aus und bereitet einen Vektor für die Ähnlichkeitssuche vor.
 */
void initProttaskVektor() {
    std::vector<char*> taskseqnames = readFilenamesFromDirectory(foldername_seq);
    for (char* seqname : taskseqnames) {
        PrototypTask t = readProttypTaskSeqfile(seqname);
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
        AnwTask t = readAnwTaskSeqfile(seqname);
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
AnwTask logBestTask(AnwTask appTask) {
    appTask.calcBestTask();
    logfileSearch << "Der ähnlichste ProttypTask für den AppTask " << appTask.name << " ist " << appTask.bestName << "(" << appTask.besthit << " Treffer)\n\n";
    return appTask;
}

void logBestTasks(AnwTask appTask) {
    //appTask.calcBestTask();
    logfileSearch << "Die ähnlichste ProttypTask für den AppTask " << appTask.name << " sind: " << "\n";

    for (std::string s : appTask.resultOneToOne.list) {
        logfileSearch << s << "\n";
    }

}

bool checkSequenzfiles() {

    if (prottaskVektor.empty()) {
        printf("Der Ordner %s enthält keine Sequenzen (Anwendung prototyptaskscpp ausgeführt?)", foldername_seq);
        return false;
    }

    if (apptaskVektor.empty()) {
        printf("Der Ordner %s enthält keine Sequenzen! (Anwendung apptasks ausgeführt?)", foldername_appseq);
        return false;
    }
    return true;
}

void initTaskVektors() {
    initProttaskVektor();
    initAppTaskVektor();
}

void compareAppTaskProtTasksOneToOne() {
    initTaskVektors();

    if (checkSequenzfiles()==false) {
        return;
    }

    openLogfileSearch();

    int count = 0; // test
    for (AnwTask t : apptaskVektor) {

        t = analyseAppTask(t);
        logfileSearch << "\n";
        logBestTask(t);

        count++;
        if (count==1) {
            break; // zu Testzwecken
        }

    }
    closeLogFileSearch();
}

AnwTask compareAppTaskWithPrototypTasksMany(AnwTask appTask, PrototypTask protTypTask ) {

    int maxIndex = appTask.maxIndex(protTypTask.sequenzen.size());

    // Iteration durch Befehle des prottypTasks.
    for (std::string protTaskSequenceEntry : protTypTask.sequenzen) {
        appTask = compareProtTaskSequenEntryWithAppTaskEntryMany(protTaskSequenceEntry, appTask, protTypTask, maxIndex);
    }

    int anzahl_hits = 0;
    for (bool found : appTask.getTaskHitList(protTypTask)) {
        if (found) {
            anzahl_hits++;
        }
    }
    logSearch(appTask.name, protTypTask.name, anzahl_hits, appTask.sequenzen.size(), protTypTask.sequenzen.size(), 1 );

    if (appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.name][protTypTask.name] == 0 || appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.name][protTypTask.name] < anzahl_hits) {
        appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.name][protTypTask.name]=anzahl_hits;
    }
    return appTask;
}

AnwTask calcBestTaskMany(AnwTask appTask) {
    std::map<std::string, int>::iterator it;
    for (it = appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.name].begin(); it != appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.name].end(); it++) {
        std::string pttName = it->first;
        int anzahlTreffer = it->second;

        PrototypTask task;
        for (PrototypTask t : prottaskVektor) {
            if (t.name == pttName) {
                task = t;
                break;
            }
        }

        float percent = (float) anzahlTreffer * 100 / task.sequenzen.size();
        if (percent>appTask.besthit) {
            appTask.besthit=percent;
            appTask.bestName=pttName;
        }
    }

    return appTask;
}

float calcSequenzSize() {
    int summe = 1;

    for (PrototypTask protTypTask : prottaskVektor) {
        summe+=protTypTask.sequenzen.size();
    }

    return (float) summe / prottaskVektor.size();
}

/**
 * Für den übergebenen Anwendungstask wird eine Ähnlichkeitssuche ausgeführt. Dabei werden nacheinander mit den vorhandenen Prototyptasks vergleichen.
 *
 * @param appTask zu untersuchender Anwendungstask.
 */
AnwTask analyseAppTaskMany(AnwTask appTask) {
    std::cout << "\nAppTask " << appTask.name << " wird geprüft!\n";

    int sizeAppDivProt = appTask.sequenzen.size() / calcSequenzSize();

    for (int i = 0; i< sizeAppDivProt; i++) {
        std::cout << "(Durchgang " << (i+1) << " von " << sizeAppDivProt << ")\n";

        for (PrototypTask protTypTask : prottaskVektor) {
            appTask.initTaskHitList(protTypTask);
            std::cout << "Vergleich mit protTypTask " << protTypTask.name << "\n";
            appTask = compareAppTaskWithPrototypTasksMany(appTask, protTypTask);
        }

        appTask = calcBestTaskMany(appTask);
        appTask.resultOneToOne.list.push_back(appTask.bestName);

        appTask.mergeFound(appTask.resultOneToOne.pptFoundMapWithBool[appTask.bestName]);

        appTask.resultOneToOne.resetPptFoundMap(); // wieder zurücksetzen!
        appTask.resultOneToOne.protTaskAnzTrefferMap.clear();
        appTask.bestName="";
        appTask.besthit=0;
    }


    for (std::string s : appTask.resultOneToOne.list) {
        std::cout << "Die besten Tasks sind:" << s;
    }
    return appTask;
}

void compareAppTaskProtTasksOneToMany() {
    initTaskVektors();
    if (checkSequenzfiles()==false) {
        return;
    }
    openLogfileSearch();
    AnwTask task = apptaskVektor[3];//erstmal nur einen!
    task = analyseAppTaskMany(task);
    logBestTasks(task);
    closeLogFileSearch();
}