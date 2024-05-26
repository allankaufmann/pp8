
#include <set>
#include "../include/taskmapperLog.h"
#include <string.h> //strtok
#include "../include/tools.hpp"
#include "../include/constants.h"
#include <sys/stat.h> //mkdir, chmod

char* currentLogfileName;
std::vector<AnwTask> apptaskVektor;
std::map<std::string, AnwTask> apptaskMap; // Abbildung der Anwendungstask auf Map
std::ofstream logfileTaskmapper;
PrototypTask currentPrototypTask;//für Diff/Paarsuche
std::vector<PrototypTask> prottaskVektor;
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
    t.taskfilename=file;
    char c_taskname[strlen(file)];
    extractTaskNameFromFileName(file, c_taskname);
    t.taskname=c_taskname;

    t.sequenzen=readSeqFile(file, t);
    return t;
}

AnwTask readAnwTaskSeqfile(const char* file) {
    AnwTask t;
    t.taskfilename=file;
    char c_taskname[strlen(file)];
    extractAppTaskNameFromFileName(file, c_taskname);
    t.taskname=c_taskname;

    t.sequenzen=readSeqFile(file, t);
    t.resetFound();
    return t;
}


/**
 * Liest die Sequenzdateien der prototypischen Tasks aus und bereitet einen Vektor für die Ähnlichkeitssuche vor.
 */
void initProttaskVektor() {
    std::vector<char*> taskseqnames = readFilenamesFromDirectory(foldername_seq);
    for (char* seqname : taskseqnames) {
        PrototypTask t = readProttypTaskSeqfile(seqname);
        prottaskVektor.push_back(t);
        printf("Der Task %s enthält %lu Einträge\n", t.taskname.c_str(), t.sequenzen.size());
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
        apptaskMap[t.taskname]=t;
        printf("Der Task %s enthält %lu Einträge\n", t.taskname.c_str(), t.sequenzen.size());
    }
    printf("%s", "\n");
}


void initTaskVektors() {
    initProttaskVektor();
    initAppTaskCollections();
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

void openLogfileTaskmapper() {
    mkdir(logfolder_taskmapper, 0777);
    currentLogfileName = getFilename(logfolder_taskmapper, logfileext);
    logfileTaskmapper.open(currentLogfileName);
}

void closeLogFileTaskmapper() {
    logfileTaskmapper.close();
}



void logMessageOnTaskmapperFileAndCout(std::string text, bool withLogFileTaskmapper){
    std::cout << text;
    if (withLogFileTaskmapper) {
        logfileTaskmapper << text;
    }
}

/**
 * Aus einer vorherigen Abbildung werden zunächst die 1:N-Einträge eines Modells entfernt, damit bei der Abbildung kein Mix aus alten und neuen Werten entsteht!
 *
 * @param model das Modell, das entfernt werden soll
 */
void dropModelInTaskmapFile(std::string model) {
    std::ifstream infile(filename_taskmap_result_from_folder);
    std::list<std::string> lines;
    std::string currentSection;
    std::string currentModel;

    std::string line;
    while (std::getline(infile, line)) {
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            currentSection = line.substr(1, line.length() - 2);
            lines.push_back(line);
            continue;
        }

        if (currentSection == result_section_one2many) {
            size_t delimiterPos = line.find('=');
            std::string keyOfCurrentLine = (delimiterPos != 0) ? line.substr(0, delimiterPos) : NULL;
            std::string valueOfCurrentLine = (delimiterPos != 0) ? line.substr(delimiterPos + 1) : NULL;
            if (keyOfCurrentLine == result_section_model) {
                currentModel = valueOfCurrentLine;
            }
            if (currentModel != model) {
                lines.push_back(line);
            }
        } else {
            lines.push_back(line);
        }
    }
    infile.close();
    std::ofstream oufile(filename_taskmap_result_from_folder);
    for (std::string l : lines) {
        oufile << l << "\n";
    }
    oufile.close();
}

void saveLineToTaskmapFile(std::string sectionToUpdate, std::string newLineToUpdate) {
    std::ifstream infile(filename_taskmap_result_from_folder);
    std::set<std::string> logs;
    std::list<std::string> oneToOne;
    bool updateOneToOneKey = false;
    std::list<std::string> oneToMany;
    bool updateOneToManyModellEntry = false;
    bool updateOneToManyModell= false;

    std::string line;
    std::string currentSection;
    std::string currentModel;
    std::string currentModelEntry;

    size_t delimiterPosInNewLine = newLineToUpdate.find('=');
    std::string keyInNewline = (delimiterPosInNewLine!=0) ? newLineToUpdate.substr(0, delimiterPosInNewLine): NULL;
    std::string valueInNewLine = (delimiterPosInNewLine!=0) ? newLineToUpdate.substr(delimiterPosInNewLine + 1):NULL;

    while (std::getline(infile, line)) {
        size_t delimiterPos = line.find('=');
        std::string keyOfCurrentLine = (delimiterPos != 0) ? line.substr(0, delimiterPos) : NULL;
        std::string valueOfCurrentLine = (delimiterPos != 0) ? line.substr(delimiterPos + 1) : NULL;

        if (line.empty()) {
            continue;
        }
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }

        if (currentSection==result_section_log) {
            logs.insert(line);
        } else if (currentSection==result_section_one2one) {
            if (sectionToUpdate == result_section_one2one){
                if (keyInNewline == keyOfCurrentLine) {
                    updateOneToOneKey=true;
                    oneToOne.push_back(newLineToUpdate);
                } else {
                    oneToOne.push_back(line);
                }
            } else {
                oneToOne.push_back(line);
            }
        } else if (currentSection == result_section_one2many) {
            if (keyOfCurrentLine == result_section_model){
                currentModel=valueOfCurrentLine;
                oneToMany.push_back(line);
                if (currentModel==sectionToUpdate) {
                    updateOneToManyModell=true;
                }
                continue;
            }

            // anderes Model als das gesuchte, kann hinzugefügt werden
            if (currentModel!=sectionToUpdate) {
                oneToMany.push_back(line);
                continue;
            }

            // ab hier nur das Modell, das aktualisiert werden soll. Es wird der Modelleintrag betrachtet
            currentModelEntry = keyOfCurrentLine;

            // Es gibt bereits einen Modelleintrag. Die aktuelle Zeile soll ersetzt werden
            if (currentModelEntry==keyInNewline) {
                updateOneToManyModellEntry=true;
                oneToMany.push_back(newLineToUpdate);
                continue;
            } else if (keyOfCurrentLine != result_section_end_model){
                oneToMany.push_back(line);
            }

            // Es gibt keinen Modelleintrag
            if (keyOfCurrentLine == result_section_end_model) {
                if (!updateOneToManyModellEntry) {
                    oneToMany.push_back(newLineToUpdate);
                }
                oneToMany.push_back(line);
                continue;
            }

        }
    }

    infile.close();

    std::ofstream oufile(filename_taskmap_result);

    oufile << "[" << result_section_log << "]\n";

    oufile << currentLogfileName << "\n";
    for (std::string l : logs) {
        oufile << l << "\n";
    }

    oufile << "\n";
    oufile << "[" << result_section_one2one << "]\n";
    for (std::string l : oneToOne) {
        oufile << l << "\n";
    }
    if (sectionToUpdate==result_section_one2one && !updateOneToOneKey) {
        oufile << newLineToUpdate << "\n";
    }

    oufile << "\n";
    oufile << "[" << result_section_one2many << "]\n";
    for (std::string l : oneToMany) {
        oufile << l << "\n";
    }

    if(sectionToUpdate!=result_section_one2one && !updateOneToManyModell) {
        oufile << result_section_model << "=" << sectionToUpdate << "\n";
        oufile << newLineToUpdate << "\n";
        oufile << "end_model\n";
    }


    oufile.close();
}

void logBestTasks(AnwTask appTask) {
    logMessageOnTaskmapperFileAndCout(
            "\nDer Anwendungstask " + appTask.taskname + " wurde auf folgende prototypischen Tasks abgebildet:\n", true);

    int countAlleTasks = 0;

    std::map<std::string, int>::iterator it;
    for (it = appTask.resultOneToOne.abgebildeteTaskMap.begin(); it != appTask.resultOneToOne.abgebildeteTaskMap.end(); it++) {
        countAlleTasks+=it->second;
    }

    dropModelInTaskmapFile(appTask.taskname);
    for (it = appTask.resultOneToOne.abgebildeteTaskMap.begin(); it != appTask.resultOneToOne.abgebildeteTaskMap.end(); it++) {
        std::string key = it->first;
        int anzahl = it->second;
        float percent = ((float) (anzahl * 100 / countAlleTasks))*0.01;
        logMessageOnTaskmapperFileAndCout(
                key + " (" + std::to_string(anzahl) + " mal zugeordnet, in %: " + std::to_string(percent) + ")\n", true);
        std::string line = " " + key+"=" + std::to_string(percent);
        saveLineToTaskmapFile(appTask.taskname, line);
    }

    float percent = (float) appTask.countFound() * 100 / appTask.sequenzen.size();
    logMessageOnTaskmapperFileAndCout(
            std::to_string(appTask.countFound()) + " von " + std::to_string(appTask.sequenzen.size()) + " gefunden! (" +
            std::to_string(percent) + "%)", true);

}

/**
 * Wird nach Ausführung der Ähnlichkeitssuche eines Anwendungstasks ausgeführt. Zu dem übergebenen Anwendungstask soll der Prototyptask ausgegeben werden, der dem Anwendungstask am ähnlichsten ist.
 *
 * @param appTask für diesen Anwendungstask wird das Ergebnis geprüft.
 */
AnwTask logBestTask(AnwTask appTask, bool percent, bool savetofile) {
    logMessageOnTaskmapperFileAndCout(
            "Der ähnlichste ProttypTask für den AppTask " + appTask.taskname + " ist " + appTask.bestName + "(" +
            std::to_string(appTask.besthit) + " " + (percent ? "%" : "") + " Treffer)\n\n", true);

    if (savetofile) {
        saveLineToTaskmapFile(result_section_one2one, appTask.taskname + "=" + appTask.bestName);
    }
    return appTask;
}

void transferTaskMapToEpEBench() {
    std::ifstream infile(filename_taskmap_result_from_folder);
    std::string currentSection;
    std::string currentModel;

    std::string line;
    std::map<std::string, std::list<std::string>> mapOfLine;

    if (!infile.is_open()) {
        std::cout << "Taskmapdatei " << filename_taskmap_result_from_folder << " nicht gefunden!\n";
        return;
    } else {
        std::cout << "Taskmapdatei " << filename_taskmap_result_from_folder << " gefunden!\n";
    }

    while (std::getline(infile, line)) {
        size_t delimiterPos = line.find('=');
        std::string keyInline = (delimiterPos!=0) ? line.substr(0, delimiterPos): NULL;
        std::string valueInLine = (delimiterPos!=0) ? line.substr(delimiterPos + 1):NULL;
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }
        if (currentSection == result_section_one2many) {
            if (keyInline == result_section_model) {
                currentModel = valueInLine;
                mapOfLine[currentModel].push_back(line);
                continue;
            }
            mapOfLine[currentModel].push_back(line);
        } else {
            continue;
        }
    }
    infile.close();

    std::list<std::string> linesEbmodels;

    currentModel = "";
    std::ifstream ebmodelsfile(filename_epebench_ebmodels);

    if (!ebmodelsfile.is_open()) {
        std::cout << "epEBench-ebmodels-Datei " << filename_epebench_ebmodels << " nicht gefunden!\n";
        return;
    } else {
        std::cout << "epEBench-ebmodels-Datei " << filename_epebench_ebmodels << " gefunden!\n";
    }

    std::set<std::string> modellsInepeBench;

    bool skipModel = false;
    while (std::getline(ebmodelsfile, line)) {
        size_t delimiterPos = line.find('=');
        std::string keyInline = (delimiterPos!=0) ? line.substr(0, delimiterPos): NULL;
        std::string valueInLine = (delimiterPos!=0) ? line.substr(delimiterPos + 1):NULL;

        if (keyInline == result_section_model) {
            currentModel = valueInLine;
            modellsInepeBench.insert(currentModel);
            if (mapOfLine[currentModel].size()==0){
                linesEbmodels.push_back(line);
                continue;
            } else if (!skipModel){
                skipModel=true;
                for (std::string s : mapOfLine[currentModel]) {
                    linesEbmodels.push_back(s);
                }
            }
        } else if (keyInline == result_section_end_model || keyInline == result_section_end_model_space) {
            if (skipModel) {
                skipModel=false;
                continue;
            }
            linesEbmodels.push_back(line);
        } else if (!skipModel) {
            linesEbmodels.push_back(line);
        }
    }
    ebmodelsfile.close();

    // Fehlende Modelle hinzufügen!
    std::map<std::string, std::list<std::string>>::iterator it;
    for (it = mapOfLine.begin(); it != mapOfLine.end(); ++it) {
        std::string key = it->first;
        std::list<std::string> values = it->second;
        if (modellsInepeBench.find(key) == modellsInepeBench.end()) {
            std::cout << key << " wird übertragen!\n";
            linesEbmodels.insert(linesEbmodels.end(), values.begin(), values.end());
        }

    }

    std::ofstream oufile(filename_epebench_ebmodels);
    for (std::string s : linesEbmodels) {
        oufile << s << "\n";
    }
    oufile.close();
    std::cout << "Taskmap wurde nach epEBench übertragen!\n";
}


