#include <vector>
#include "../include/taskclasses.h"
#include "../include/taskmapperLog.h"
#include <iostream>

std::vector<PrototypTask> newprottaskVektor;

PrototypTask analyseTaskType(PrototypTask prototypTask) {
    currentPrototypTask = prototypTask;
    currentPrototypTask.initHit();

    std::string prevEntry = currentPrototypTask.sequenzen[0];
    std::string currentEntry;

    std::map<std::string, std::map<std::string, bool>> pairEntryMap;

    for (int i = 1; i < currentPrototypTask.sequenzen.size(); i++) {
        currentEntry=currentPrototypTask.sequenzen[i];
        currentPrototypTask.uniqueEntryPairMap[prevEntry][currentEntry]=true;
        pairEntryMap[prevEntry][currentEntry]=true;
        prevEntry=currentEntry;
    }

    for (PrototypTask prototypTaskToCompare : prottaskVektor) {
        if (currentPrototypTask.taskname == prototypTaskToCompare.taskname) {
            continue;
        }

        prevEntry = prototypTaskToCompare.sequenzen[0];
        for (int i = 1; i < prototypTaskToCompare.sequenzen.size(); i++) {
            currentEntry=prototypTaskToCompare.sequenzen[i];
            if (pairEntryMap[prevEntry][currentEntry]) {
                pairEntryMap[prevEntry][currentEntry]=false;
                prevEntry=currentEntry;
            }
        }
    }

    std::map<std::string, std::map<std::string, bool>>::iterator it;
    //std::cout << "Nicht gefunden wurden folgende Kombinationen:\n";
    int counter = 0;
    int counterNot = 0;
    for (it = pairEntryMap.begin(); it != pairEntryMap.end(); it++) {
        std::string prev = it->first;
        std::map<std::string, bool> innerMap = it->second;
        std::map<std::string, bool>::iterator innerIt;
        for (innerIt = innerMap.begin(); innerIt != innerMap.end(); innerIt++) {
            std::string current = innerIt->first;
            bool b = innerIt->second;
            if (b) {
                currentPrototypTask.uniqueEntryPairMap[prev][current]=true;
                counter++;
                //std::cout << "[" << prev << "/" << current << "]\n";
            } else {
                counterNot++;
            }
        }
    }

    logMessageOnTaskmapperFileAndCout("Im Tasktyp " + currentPrototypTask.taskname + " sind insgesamt " + std::to_string(counter) + " Kombinationen eindeutig,  nicht eindeutig sind: " + std::to_string(counterNot) + "\n", true);

    return currentPrototypTask;
}

AnwTask analyseAppTaskPair(AnwTask anwTak) {

    std::string prevEntry = anwTak.sequenzen[0];
    std::string currentEntry;

    std::map<std::string, std::map<std::string, bool>> pairEntryMap;

    for (int i = 1; i < anwTak.sequenzen.size(); i++) {
        currentEntry=anwTak.sequenzen[i];
        pairEntryMap[prevEntry][currentEntry]=true;
        prevEntry=currentEntry;
    }

    for (PrototypTask t : newprottaskVektor) {
        int counter = 0;
        std::map<std::string, std::map<std::string, bool>>::iterator it;
        for(it=t.uniqueEntryPairMap.begin(); it != t.uniqueEntryPairMap.end(); it++) {
            std::string prev = it->first;
            std::map<std::string, bool> innerMap = it->second;
            std::map<std::string, bool>::iterator innerIt;
            for(innerIt=innerMap.begin(); innerIt!=innerMap.end(); innerIt++) {
                std::string current = innerIt->first;
                if (pairEntryMap[prev][current]) {
                    t.uniqueEntryPairMap[prev][current]=true;
                    counter++;
                }
            }
        }
        logMessageOnTaskmapperFileAndCout("Im Anwendungstask " + anwTak.taskname + " sind aus " + t.taskname + " " + std::to_string(counter) + " eindeutige Kombinationen enthalten\n", true);

        if (anwTak.besthit<counter) {
            anwTak.bestName=t.taskname;
            anwTak.besthit=counter;
        }
    }

    logMessageOnTaskmapperFileAndCout("Für den Anwendungstask " + anwTak.taskname + " ist der Tasktyp " + anwTak.bestName + " bei Paarsuche am ähnlichsten: " + std::to_string(anwTak.besthit) + " Treffer!\n\n", true);

    return anwTak;
}

void testPairSearch() {
    initTaskVektors();
    if (checkSequenzfiles()==false) {
        return;
    }

    openLogfileTaskmapper();

    for (PrototypTask t : prottaskVektor) {
        newprottaskVektor.push_back(
                analyseTaskType(t)
                );
    }

    std::cout << "\n";

    for(AnwTask a : apptaskVektor) {
        analyseAppTaskPair(a);
    }

    closeLogFileTaskmapper();

}


void testTripleSearch() {
    initTaskVektors();
    if (checkSequenzfiles()==false) {
        return;
    }

    currentPrototypTask = prottaskVektor[1];
    currentPrototypTask.initHit();

    std::string prevPrevEntry = currentPrototypTask.sequenzen[0];
    std::string prevEntry = currentPrototypTask.sequenzen[1];
    std::string currentEntry;

    std::map<std::string, std::map<std::string, std::map<std::string, bool>>> myMap;

    for (int i = 2; i < currentPrototypTask.sequenzen.size(); i++) {
        currentEntry=currentPrototypTask.sequenzen[i];
        myMap[prevPrevEntry][prevEntry][currentEntry]=true;
        prevPrevEntry=prevEntry;
        prevEntry=currentEntry;
    }


    for (PrototypTask prototypTaskToCompare : prottaskVektor) {
        if (currentPrototypTask.taskname == prototypTaskToCompare.taskname) {
            continue;
        }

        std::cout << "Triplesuche " << currentPrototypTask.taskname << " mit " << prototypTaskToCompare.taskname << "\n";

        prevPrevEntry = prototypTaskToCompare.sequenzen[0];
        prevEntry = prototypTaskToCompare.sequenzen[1];
        for (int i = 2; i < prototypTaskToCompare.sequenzen.size(); i++) {
            currentEntry=prototypTaskToCompare.sequenzen[i];
            if (myMap[prevPrevEntry][prevEntry][currentEntry]) {
                myMap[prevPrevEntry][prevEntry][currentEntry]=false;
                prevPrevEntry=prevEntry;
                prevEntry=currentEntry;
            }
        }
    }

    std::map<std::string, std::map<std::string, std::map<std::string, bool>>> ::iterator it;
    std::cout << "Nicht gefunden wurden folgende Kombinationen:\n";
    int counter = 0;
    int counterNot = 0;
    for (it = myMap.begin(); it != myMap.end(); it++) {
        std::string prevprev = it->first;
        std::map<std::string, std::map<std::string, bool>> innerMap = it->second;
        std::map<std::string, std::map<std::string, bool>>::iterator innerIt;
        for (innerIt = innerMap.begin(); innerIt != innerMap.end(); innerIt++) {
            std::string prev = innerIt->first;
            std::map<std::string, bool> innerinnerMap = innerIt->second;
            std::map<std::string, bool>::iterator  innerInnerIt;
            for (innerInnerIt = innerinnerMap.begin(); innerInnerIt != innerinnerMap.end(); innerInnerIt++) {
                std::string current = innerInnerIt->first;
                bool b = innerInnerIt->second;
                if (b) {
                    counter++;
                    std::cout << "[" << prevprev << "/" << prev << "/" << current << "]\n";
                } else {
                    counterNot++;
                }

            }
        }
    }
    std::cout << "Demnach sind im Tasktyp '" << currentPrototypTask.taskname << "' insgesamt " << counter << " Kombinationen eindeutig,  nicht eindeutig sind: " << counterNot << "\n";
    // im Test wurden 570 eindeutige und 1224 nicht eindeutige gefunden. Triple sind nicht geeignet!

}