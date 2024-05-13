
#include <string.h>
#include "Result.h"
#include "taskclasses.h"
#include "taskmapperLog.h"
#include <thread>
#include "math.h" //ceil
AnwTask calcBestTaskMany(AnwTask appTask) {

    for (const auto& kv : appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.taskname]) {
        std::string pttName = kv.first;
        int anzahlTreffer = kv.second;

        PrototypTask task;
        for (PrototypTask t : prottaskVektor) {
            if (t.taskname.compare(pttName)==0) {
                task = t;
                break;
            }
        }

        float percent = (float) anzahlTreffer * 100 / task.sequenzen.size();

        int indexFirstFalse = 0;

        std::vector<bool> boolVektor = appTask.resultOneToOne.pptFoundMapWithBool[task.taskname];

        for (int i = 0; i<boolVektor.size(); i++) {
            if (!boolVektor[i]) {
                indexFirstFalse=i;
                break;
            }
        }


        if (percent==appTask.besthit) {
            logMessageOnTaskmapperFileAndCout("Gleichheit mit " + task.taskname + "\n", true);
            if (indexFirstFalse<appTask.bestFirstFalseIndex) {
                logMessageOnTaskmapperFileAndCout(
                        "Erster false wert kleiner! Der Task " + task.taskname + " wird verwendet!", true);
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
        if (appTask.resultOneToOne.pptFoundMapWithBool[protTypTask.taskname][index]) {
            continue; // Sequenz bereits gefunden, nächster Treffer!
        }
        if (appTask.found[index]) {
            continue; // Sequenz bereits gefunden, nächster Treffer!
        }
        if (maxIndex!=0 && index > maxIndex) {
            return appTask;
        }

        appTask.resultOneToOne.pptFoundMapWithBool[protTypTask.taskname][index]=true;

        return appTask;
    }

    return appTask;
}

AnwTask compareAppTaskWithPrototypTasksMany(AnwTask appTask, PrototypTask protTypTask, int maxIndex ) {
    appTask.initIndexOfMap(maxIndex, protTypTask.taskname);

    for (int i = 0; i < protTypTask.sequenzen.size(); i++) {
        appTask = compareProtTaskSequenEntryWithAppTaskEntryMany(protTypTask.sequenzen[i], appTask, protTypTask, maxIndex);
        appTask.initIndexOfMap(maxIndex, protTypTask.taskname);
    }



    int anzahl_hits = 0;
    for (bool found : appTask.getTaskHitList(protTypTask)) {
        if (found) {
            anzahl_hits++;
        }
    }

    logMessageOnTaskmapperFileAndCout(
            "In AppTask " + appTask.taskname + "(Gesamt: " + std::to_string(appTask.sequenzen.size()) +
            ") sind aus ProtTask " + protTypTask.taskname + " (Gesamt: " + std::to_string(protTypTask.sequenzen.size()) +
            ")  " + std::to_string(anzahl_hits) + " Einträge vorhanden!\n\n", true);
    logfileTaskmapper.flush();

    if (appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname] == 0 || appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname] < anzahl_hits) {
        appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname]=anzahl_hits;
    }
    return appTask;
}

AnwTask prepareAnwTaskAndProtTypTaskForCompare(AnwTask appTask, PrototypTask protTypTask ) {
    int maxIndex = appTask.lastIndexOfTrue() + protTypTask.sequenzen.size();
    appTask.initTaskHitList(protTypTask);
    logMessageOnTaskmapperFileAndCout("Vergleich " + appTask.taskname + " mit protTypTask " + protTypTask.taskname + "\n",
                                      false);
    appTask = compareAppTaskWithPrototypTasksMany(appTask, protTypTask, maxIndex);
    resultOneToMany.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname]=appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname];
    resultOneToMany.pptFoundMapWithBool[protTypTask.taskname]=appTask.resultOneToOne.pptFoundMapWithBool[protTypTask.taskname];
    return appTask;
}

AnwTask analyse(AnwTask appTask) {
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
    logBestTask(appTask, true, false);
    appTask.resultOneToOne.list.push_back(appTask.bestName);
    appTask.resultOneToOne.abgebildeteTaskMap[appTask.bestName]+=1;

    appTask.mergeFound(appTask.resultOneToOne.pptFoundMapWithBool[appTask.bestName]);
    appTask.resultOneToOne.resetPptFoundMap(appTask.resultOneToOne.pptFoundMapWithBool[appTask.bestName]); // wieder zurücksetzen!
    appTask.resultOneToOne.protTaskAnzTrefferMap.clear();
    appTask.bestName="";
    appTask.besthit=0;
    return appTask;
}

/**
 * Für den übergebenen Anwendungstask wird eine Ähnlichkeitssuche ausgeführt. Dabei werden nacheinander mit den vorhandenen Prototyptasks vergleichen.
 *
 * @param appTask zu untersuchender Anwendungstask.
 */
AnwTask analyseAppTaskMany(AnwTask appTask) {
    logMessageOnTaskmapperFileAndCout("\nAppTask " + appTask.taskname + " wird geprüft!\n", true);

    int sizeAppDivProt = ceil(float(appTask.sequenzen.size() / calcSequenzSize()))+1; // Produkt wird immer aufgerundet!

    for (int i = 0; i< sizeAppDivProt; i++) {
        resultOneToMany=appTask.resultOneToOne;
        logMessageOnTaskmapperFileAndCout(
                "(Durchgang " + std::to_string(i + 1) + " von " + std::to_string(sizeAppDivProt) + ")\n", true);

        if (appTask.allFound()){
            return appTask; // wenn im letzten Durchgang alle gefunden wurden, dann wird dieser Durchgang abgebrochen!
        }

        appTask= analyse(appTask);
    }
    float percentFound = appTask.percentFound();
    if (percentFound < 85) {
        float newpercentFound = 0;
        do {
            percentFound = appTask.percentFound();
            logMessageOnTaskmapperFileAndCout("Der Anwendungstask wurde zu " + std::to_string(percentFound) + "% abgedeckt. Es werden weitere Runden durchgeführt, um eine höhere Abdeckung zu erreichen!\n", true);
            appTask= analyse(appTask);
            newpercentFound = appTask.percentFound();

            float diff = newpercentFound - percentFound;

            if (diff<2) {
                break;
            }
        } while ((newpercentFound > percentFound && newpercentFound < 85) || newpercentFound==percentFound);
    }


    return appTask;
}


void compareAppTaskProtTasksOneToMany() {
    initTaskVektors();
    if (checkSequenzfiles()==false) {
        return;
    }
    openLogfileTaskmapper();

    for (AnwTask t : apptaskVektor) {
        t = analyseAppTaskMany(t);
        logBestTasks(t);
    }
    closeLogFileTaskmapper();
}

void compareAppTaskProtTasksOneToManyTest(std::string appTaskName) {

    if (checkSequenzfiles()==false) {
        return;
    }
    openLogfileTaskmapper();
    AnwTask task = apptaskMap[appTaskName];
    if (!task.taskname.empty()) {
        task = analyseAppTaskMany(task);
        logBestTasks(task);
    } else {
        logMessageOnTaskmapperFileAndCout("Anwendungs task [" + appTaskName + "] nicht gefunden!", true);
    }

    closeLogFileTaskmapper();
}
