

Result resultOneToOne;

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


void logTaskMapperOne2One(std::string taskname, std::string ptname, int size, int sizeAppTask, int sizeProtTask, int div) {
    logfileTaskmapper << "In AppTask " << taskname << "(Gesamt: " << sizeAppTask << ") sind aus ProtTask " << ptname << " (Gesamt: " << sizeProtTask << ")  " << size << " Einträge vorhanden! (Der Prottask wurde aufgrund der Größe des AppTask " << div << " mal wiederholt!)\n";
    logfileTaskmapper.flush();
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
            std::cout << "\n(Durchgang " << i << " von " << sizeAppDivProt << "; " << appTask.taskname<< " mit " << protTypTask.taskname << ") \n";
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
    logTaskMapperOne2One(appTask.taskname, protTypTask.taskname, anzahl_hits, appTask.sequenzen.size(),
                         protTypTask.sequenzen.size(), sizeAppDivProt);

    if (appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname] == 0 || appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname] < anzahl_hits) {
        appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname]=anzahl_hits;
    }

    appTask.resetFound();
    return appTask;
}

AnwTask prepareAnwTaskAndProtTypTaskForCompareOneToOne(AnwTask appTask, PrototypTask protTypTask ) {
    logMessageOnTaskmapperFileAndCout("Vergleich " + appTask.taskname + " mit protTypTask " + protTypTask.taskname + "\n",
                                      false);
    appTask = compareAppTaskWithPrototypTasks(appTask, protTypTask);
    resultOneToOne.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname]=appTask.resultOneToOne.protTaskAnzTrefferMap[appTask.taskname][protTypTask.taskname];
    return appTask;
}

/**
 * Für den übergebenen Anwendungstask wird eine Ähnlichkeitssuche ausgeführt. Dabei werden nacheinander mit den vorhandenen Prototyptasks vergleichen.
 *
 * @param appTask zu untersuchender Anwendungstask.
 */
AnwTask analyseAppTask(AnwTask appTask) {
    logMessageOnTaskmapperFileAndCout("\nAppTask " + appTask.taskname + " wird geprüft!\n", true);
    //std::cout << "\nAppTask " << appTask.taskfilename << " wird geprüft!\n";

    std::thread myThreads[prottaskVektor.size()];
    int counter = 0;
    for (PrototypTask protTypTask : prottaskVektor) {
        myThreads[counter] = std::thread(prepareAnwTaskAndProtTypTaskForCompareOneToOne, appTask, protTypTask);
        counter++;
    }

    for (int j = 0; j < prottaskVektor.size(); j++) {
        myThreads[j].join();
    }

    appTask.resultOneToOne=resultOneToOne;

    return appTask;
}


void compareAppTaskProtTasksOneToOne() {
    initTaskVektors();

    if (checkSequenzfiles()==false) {
        return;
    }

    openLogfileTaskmapper();

    for (AnwTask t : apptaskVektor) {

        t = analyseAppTask(t);
        logfileTaskmapper << "\n";
        t.calcBestTask();
        logBestTask(t, false, true);
    }
    closeLogFileTaskmapper();
}

void compareAppTaskProtTasksOneToOneTest(std::string appTaskName) {
    if (checkSequenzfiles()==false) {
        return;
    }

    openLogfileTaskmapper();
    AnwTask task = apptaskMap[appTaskName];
    task = analyseAppTask(task);
    logfileTaskmapper << "\n";
    task.calcBestTask();
    logBestTask(task, false, true);
    closeLogFileTaskmapper();
}