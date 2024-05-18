
#include "../include/taskmapperDiffsearch.h"
std::vector<std::string> similarTasks;

PrototypTask compareProtTaskSequenEntries(int indexToCompare, PrototypTask protTypTaskToCompare) {
    std::string protTaskSequenceEntry = currentPrototypTask.sequenzen[indexToCompare];

    for (int i=0; i < protTypTaskToCompare.sequenzen.size(); i++) {
        if (protTypTaskToCompare.hit[i] == true) {
            continue; // Sequenz bereits gefunden, nächster Treffer!
        }

        if (protTypTaskToCompare.sequenzen[i].compare(protTaskSequenceEntry) == 0) {
            currentPrototypTask.hit[indexToCompare]=true;
            protTypTaskToCompare.hit[i]=true; // Sequenz gefunden, wird auf true gesetzt!
            return protTypTaskToCompare;
        }
    }
    return protTypTaskToCompare;
}

void comparePrototypTaskWithOtherPrototypTask(PrototypTask protTypTaskToCompare) {
    for (int i = 0; i < currentPrototypTask.sequenzen.size(); i++) {
        if (currentPrototypTask.hit[i]) {
            continue;
        }
        protTypTaskToCompare = compareProtTaskSequenEntries(i, protTypTaskToCompare);
    }
}

bool isSimilarTask(std::string taskname) {
    for (std::string  s : similarTasks) {
        if (s == taskname) {
            return true;
        }
    }
    return false;
}

void testDiffSearch() {
    initTaskVektors();
    if (checkSequenzfiles()==false) {
        return;
    }
    currentPrototypTask = prottaskVektor[3];
    std::cout << "Der Tasktyp " << currentPrototypTask.taskname << " wird mit den anderen Tasktypen verglichen!\n";
    currentPrototypTask.initHit();

    for (PrototypTask ptt : prottaskVektor) {
        if (currentPrototypTask.taskname == ptt.taskname) {
            continue;
        }
        //std::cout << "Vergleiche " << currentPrototypTask.taskname << " mit " << ptt.taskname << "\n";
        ptt.initHit();
        comparePrototypTaskWithOtherPrototypTask(ptt);

        /*std::cout << "nicht enthalten sind:\n";
        for (int i = 0; i < currentPrototypTask.sequenzen.size(); i++) {
            if (!currentPrototypTask.hit[i]) {
                std::cout << currentPrototypTask.sequenzen[i] << "\n";
            }
        }*/

        std::cout << "  Aus " << currentPrototypTask.taskname << " sind in " << ptt.taskname << " nicht enthalten: " << currentPrototypTask.countNotFound() << " von " << currentPrototypTask.sequenzen.size() << "\n";
        currentPrototypTask.initHit();
    }

    std::cout << "\nEs wird nun geprüft, ob der Tasktyp '" << currentPrototypTask.taskname << "' Sequenzen enthält, die in keinem anderen Tasktyp vorkommen!\n";
    for (PrototypTask ptt : prottaskVektor) {
        if (currentPrototypTask.taskname == ptt.taskname) {
            continue;
        }
        if (isSimilarTask(ptt.taskname)) {
            continue;
        }

        std::cout << "Vergleiche " << currentPrototypTask.taskname << " mit " << ptt.taskname << "\n";
        ptt.initHit();
        comparePrototypTaskWithOtherPrototypTask(ptt);
        std::cout << "  Aus dem Tasktypen '" << currentPrototypTask.taskname << "' wurden bisher " << currentPrototypTask.countNotFound() << " nicht gefunden!\n";
    }

    std::cout << "Insgesamt nicht enthalten sind:\n";
    for (int i = 0; i < currentPrototypTask.sequenzen.size(); i++) {
        if (!currentPrototypTask.hit[i]) {
            std::cout << currentPrototypTask.sequenzen[i] << "\n";
        }
    }

    std::cout << "Das sind " << currentPrototypTask.countNotFound() << " von " << currentPrototypTask.sequenzen.size() << "\n";
}

