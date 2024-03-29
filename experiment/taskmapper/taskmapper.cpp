#include <iostream>
#include <string.h> //strtok
#include <vector>
#include "../tools.hpp"
#include "../constants.h"
#include <dirent.h>
#include <sys/stat.h> //mkdir
#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include <thread>
#include "math.h"
//const char* daddseq= "taskmapper/taskseq/dadd.seq";
//const char* sampleappseq = "taskmapper/appseq/sampleapp.seq";



std::ofstream logfileTaskmapper;
static const char *const logfolder_taskmapper = "logs/taskmapper/";

void logMessageOnTaskmapperFileAndCout(std::string text, bool withLogFileTaskmapper){
    std::cout << text;
    if (withLogFileTaskmapper) {
        logfileTaskmapper << text;
    }
}


class Result {
public:
    std::map<std::string, std::map<std::string, int>> protTaskAnzTrefferMap; // <AnwTask, <Prottask, Treffer>>
    std::map<std::string, std::vector<bool>> pptFoundMapWithBool; // für jeden PPT die Treffer speichern
    std::list<std::string> list; // Namen der abgebildeten PPTs.
    std::map<std::string, int> abgebildeteTaskMap;

    void resetPptFoundMap(std::vector<bool> hit) {
        pptFoundMapWithBool.clear();
        /*std::map<std::string, std::vector<bool>>::iterator it;
        for (it = pptFoundMapWithBool.begin(); it != pptFoundMapWithBool.end(); it++) {
            std::string s = it->first;
            pptFoundMapWithBool[s]=hit;
        }*/
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
    std::map<std::string, std::list<int>> indexOfMap;

    void initIndexOfMap(int maxIndex, std::string prottaskname) {
        indexOfMap.clear();
        for (int i = 0; i < sequenzen.size(); i++) {
            if (maxIndex!=0 && i>maxIndex) {
                return;
            }
            if (found[i]){
                continue;
            }
            if (resultOneToOne.pptFoundMapWithBool[prottaskname][i]) {
                continue;
            }
            indexOfMap[sequenzen[i]].push_back(i);
        }
    }

    int lastIndexOfTrue() {
        for (int i = found.size()-1; i>0; i--) {
            if (found[i]) {
                return i;
            }
        }
        return 0;
    }


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

    bool allFound() {
        return sequenzen.size() == countFound();
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
    int bestFirstFalseIndex = 0;

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
std::map<std::string, AnwTask> apptaskMap; // Abbildung der Anwendungstask auf Map

/**
 * Für die Abbildung 1:N werden die Tasks parallel geprüft. Aus diesem Grund werden die Zwischenergebnisse hier zusammengeführt.
 */
Result resultOneToMany;

std::vector<std::string> readSeqFile(const char* file, Task t) {
    std::vector<std::string> sequenzen;

    FILE* filePointer;
    filePointer = fopen(file, "r");

    int bufferLength = 255;
    char buffer[bufferLength];

    char* task;

    while(fgets(buffer, bufferLength, filePointer)) {
        task = strtok(buffer, " ");
        task = strtok(task, "\n");
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

void openLogfileTaskmapper() {
    mkdir(logfolder_taskmapper, 0777);
    logfileTaskmapper.open(getFilename(logfolder_taskmapper));
}

void closeLogFileTaskmapper() {
    logfileTaskmapper.close();
}


void logTaskMapperOne2One(std::string taskname, std::string ptname, int size, int sizeAppTask, int sizeProtTask, int div) {
    logfileTaskmapper << "In AppTask " << taskname << "(Gesamt: " << sizeAppTask << ") sind aus ProtTask " << ptname << " (Gesamt: " << sizeProtTask << ")  " << size << " Einträge vorhanden! (Der Prottask wurde aufgrund der Größe des AppTask " << div << " mal wiederholt!)\n";
    logfileTaskmapper.flush();
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
    std::list<int> indexes = appTask.indexOfMap[protTaskSequenceEntry];

    if (indexes.empty()) {
        return appTask;
    }

    for (int index : indexes) {
        if (appTask.resultOneToOne.pptFoundMapWithBool[protTypTask.name][index]) {
            continue; // Sequenz bereits gefunden, nächster Treffer!
        }
        if (appTask.found[index]) {
            continue; // Sequenz bereits gefunden, nächster Treffer!
        }
        if (maxIndex!=0 && index > maxIndex) {
            return appTask;
        }

        appTask.resultOneToOne.pptFoundMapWithBool[protTypTask.name][index]=true;

        return appTask;
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
            std::cout << "\n(Durchgang " << i << " von " << sizeAppDivProt << ")\n";
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
    logTaskMapperOne2One(appTask.name, protTypTask.name, anzahl_hits, appTask.sequenzen.size(),
                         protTypTask.sequenzen.size(), sizeAppDivProt);

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
    logMessageOnTaskmapperFileAndCout("\nAppTask " + appTask.name + " wird geprüft!\n", true);
    //std::cout << "\nAppTask " << appTask.name << " wird geprüft!\n";

    for (PrototypTask protTypTask : prottaskVektor) {
        logMessageOnTaskmapperFileAndCout("Vergleich " + appTask.name + " mit protTypTask " + protTypTask.name + "\n",
                                          false);
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
void initAppTaskCollections() {
    std::vector<char*> appseqnames = readFilenamesFromDirectory(foldername_appseq);
    for (char* seqname : appseqnames) {
        AnwTask t = readAnwTaskSeqfile(seqname);
        //t.initIndexOfMap();
        apptaskVektor.push_back(t);
        apptaskMap[t.name]=t;
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
    logMessageOnTaskmapperFileAndCout(
            "Der ähnlichste ProttypTask für den AppTask " + appTask.name + " ist " + appTask.bestName + "(" +
            std::to_string(appTask.besthit) + " % Treffer)\n\n", true);
    return appTask;
}

void logBestTasks(AnwTask appTask) {
    logMessageOnTaskmapperFileAndCout(
            "\nDer Anwendungstask " + appTask.name + " wurde auf folgende prototypischen Tasks abgebildet:\n", true);

    int countAlleTasks = 0;

    std::map<std::string, int>::iterator it;
    for (it = appTask.resultOneToOne.abgebildeteTaskMap.begin(); it != appTask.resultOneToOne.abgebildeteTaskMap.end(); it++) {
        //std::string key = it->first;
        countAlleTasks+=it->second;
    }

    for (it = appTask.resultOneToOne.abgebildeteTaskMap.begin(); it != appTask.resultOneToOne.abgebildeteTaskMap.end(); it++) {
        std::string key = it->first;
        int anzahl = it->second;
        float percent = anzahl * 100 / countAlleTasks;
        logMessageOnTaskmapperFileAndCout(
                key + " (" + std::to_string(anzahl) + " mal zugeordnet, in %: " + std::to_string(percent) + ")\n", true);
    }

    float percent = appTask.countFound() * 100 / appTask.sequenzen.size();
    logMessageOnTaskmapperFileAndCout(
            std::to_string(appTask.countFound()) + " von " + std::to_string(appTask.sequenzen.size()) + " gefunden! (" +
            std::to_string(percent) + "%)", true);
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
    initAppTaskCollections();
}

void compareAppTaskProtTasksOneToOne(bool test) {
    initTaskVektors();

    if (checkSequenzfiles()==false) {
        return;
    }

    openLogfileTaskmapper();

    int count = 0; // test
    for (AnwTask t : apptaskVektor) {

        t = analyseAppTask(t);
        logfileTaskmapper << "\n";
        t.calcBestTask();
        logBestTask(t);

        if (test) { // wenn gesetzt, dann wird nur ein anwTask abgebildet
            count++;
            if (count==1) {
                break;
            }
        }
    }
    closeLogFileTaskmapper();
}

AnwTask compareAppTaskWithPrototypTasksMany(AnwTask appTask, PrototypTask protTypTask, int maxIndex ) {
    appTask.initIndexOfMap(maxIndex, protTypTask.name);

    for (int i = 0; i < protTypTask.sequenzen.size(); i++) {
        appTask = compareProtTaskSequenEntryWithAppTaskEntryMany(protTypTask.sequenzen[i], appTask, protTypTask, maxIndex);
        appTask.initIndexOfMap(maxIndex, protTypTask.name);
    }



    int anzahl_hits = 0;
    for (bool found : appTask.getTaskHitList(protTypTask)) {
        if (found) {
            anzahl_hits++;
        }
    }

    logMessageOnTaskmapperFileAndCout(
            "In AppTask " + appTask.name + "(Gesamt: " + std::to_string(appTask.sequenzen.size()) +
            ") sind aus ProtTask " + protTypTask.name + " (Gesamt: " + std::to_string(protTypTask.sequenzen.size()) +
            ")  " + std::to_string(anzahl_hits) + " Einträge vorhanden!\n\n", true);
    logfileTaskmapper.flush();

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

        int indexFirstFalse = 0;

        std::vector<bool> boolVektor = appTask.resultOneToOne.pptFoundMapWithBool[task.name];

        for (int i = 0; i<boolVektor.size(); i++) {
            if (!boolVektor[i]) {
                indexFirstFalse=i;
                break;
            }
        }


        if (percent==appTask.besthit) {
            logMessageOnTaskmapperFileAndCout("Gleichheit mit " + task.name + "\n", true);
            if (indexFirstFalse<appTask.bestFirstFalseIndex) {
                logMessageOnTaskmapperFileAndCout(
                        "Erster false wert kleiner! Der Task " + task.name + " wird verwendet!", true);
                appTask.besthit=percent;
                appTask.bestName=pttName;
                appTask.bestFirstFalseIndex=indexFirstFalse;
            }
        } else if (percent>appTask.besthit) {
            appTask.besthit=percent;
            appTask.bestName=pttName;
            appTask.bestFirstFalseIndex=indexFirstFalse;
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



AnwTask prepareAnwTaskAndProtTypTaskForCompare(AnwTask appTask, PrototypTask protTypTask ) {
    int maxIndex = appTask.lastIndexOfTrue() + protTypTask.sequenzen.size();
    appTask.initTaskHitList(protTypTask);
    logMessageOnTaskmapperFileAndCout("Vergleich " + appTask.name + " mit protTypTask " + protTypTask.name + "\n",
                                      false);
    appTask = compareAppTaskWithPrototypTasksMany(appTask, protTypTask, maxIndex);
    resultOneToMany.protTaskAnzTrefferMap[appTask.name][protTypTask.name]=appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.name][protTypTask.name];
    resultOneToMany.pptFoundMapWithBool[protTypTask.name]=appTask.resultOneToOne.pptFoundMapWithBool[protTypTask.name];
    return appTask;
}

/**
 * Für den übergebenen Anwendungstask wird eine Ähnlichkeitssuche ausgeführt. Dabei werden nacheinander mit den vorhandenen Prototyptasks vergleichen.
 *
 * @param appTask zu untersuchender Anwendungstask.
 */
AnwTask analyseAppTaskMany(AnwTask appTask) {
    logMessageOnTaskmapperFileAndCout("\nAppTask " + appTask.name + " wird geprüft!\n", true);

    int sizeAppDivProt = ceil(float(appTask.sequenzen.size() / calcSequenzSize()))+1; // Produkt wird immer aufgerundet!

    for (int i = 0; i< sizeAppDivProt; i++) {
        resultOneToMany=appTask.resultOneToOne;
        logMessageOnTaskmapperFileAndCout(
                "(Durchgang " + std::to_string(i + 1) + " von " + std::to_string(sizeAppDivProt) + ")\n", true);

        if (appTask.allFound()){
            return appTask; // wenn im letzten Durchgang alle gefunden wurden, dann wird dieser Durchgang abgebrochen!
        }

        std::list<std::thread> threadlist;
        std::thread myThreads[prottaskVektor.size()];


        for (int j = 0; j < prottaskVektor.size(); j++) {
            myThreads[j] = std::thread(prepareAnwTaskAndProtTypTaskForCompare, appTask, prottaskVektor[j]);
        }

        for (int j = 0; j < prottaskVektor.size(); j++) {
            myThreads[j].join();
        }

        appTask.resultOneToOne=resultOneToMany;
        appTask = calcBestTaskMany(appTask);
        logBestTask(appTask);
        appTask.resultOneToOne.list.push_back(appTask.bestName);
        appTask.resultOneToOne.abgebildeteTaskMap[appTask.bestName]+=1;

        appTask.mergeFound(appTask.resultOneToOne.pptFoundMapWithBool[appTask.bestName]);
        appTask.resultOneToOne.resetPptFoundMap(appTask.resultOneToOne.pptFoundMapWithBool[appTask.bestName]); // wieder zurücksetzen!
        appTask.resultOneToOne.protTaskAnzTrefferMap.clear();
        appTask.bestName="";
        appTask.besthit=0;
    }

    return appTask;
}

void compareAppTaskProtTasksOneToMany(bool test) {
    initTaskVektors();
    if (checkSequenzfiles()==false) {
        return;
    }
    openLogfileTaskmapper();

    if (test) {
        AnwTask task = apptaskVektor[3];//sobelv_20_20
        //AnwTask task = apptaskVektor[0];//sobelv
        //AnwTask task = apptaskVektor[10];//sobelv_50_50

        task = analyseAppTaskMany(task);
        logBestTasks(task);
    } else {
        logMessageOnTaskmapperFileAndCout("Noch nicht implementiert!", true);
    }

    closeLogFileTaskmapper();
}

void compareAppTaskProtTasksOneToManyTest(std::string appTaskName) {

    if (checkSequenzfiles()==false) {
        return;
    }
    openLogfileTaskmapper();
    AnwTask task = apptaskMap[appTaskName];
    if (!task.name.empty()) {
        task = analyseAppTaskMany(task);
        logBestTasks(task);
    } else {
        logMessageOnTaskmapperFileAndCout("Anwendungs task [" + appTaskName + "] nicht gefunden!", true);
    }

    closeLogFileTaskmapper();
}