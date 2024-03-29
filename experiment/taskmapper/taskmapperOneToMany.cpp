

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
