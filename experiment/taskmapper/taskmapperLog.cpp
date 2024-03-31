

std::vector<PrototypTask> prottaskVektor;
std::vector<AnwTask> apptaskVektor;
std::map<std::string, AnwTask> apptaskMap; // Abbildung der Anwendungstask auf Map
std::ofstream logfileTaskmapper;
static const char *const logfolder_taskmapper = "logs/taskmapper/";

std::string result_section_log="Log";
std::string result_section_one2one="OneToOne";
std::string result_section_many2one="ManyToOne";
std::string result_section_model="model";
std::string result_section_end_model="end_model";

char* currentLogfileName;

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

void extractTaskNameFromFileName(const char* filename, char* taskname) {
    const char *start = strrchr(filename, '/');
    const char *end = strchr(filename, '.');

    if (start != NULL && end != NULL && end > start) {
        size_t length = end - start - 1; //


        strncpy(taskname, start + 1, length);
        taskname[length] = '\0'; // Null-terminate the string
    } else {
        taskname[0] = '\0'; // If no '/' and '.', return an empty string
    }
}

PrototypTask readProttypTaskSeqfile(const char* file) {
    PrototypTask t;
    t.name=file;

    char c_taskname[strlen(file)];
    extractTaskNameFromFileName(file, c_taskname);
    t.taskname=c_taskname;
    t.sequenzen=readSeqFile(file, t);
    return t;
}

AnwTask readAnwTaskSeqfile(const char* file) {
    AnwTask t;
    t.name=file;

    char c_taskname[strlen(file)];
    extractTaskNameFromFileName(file, c_taskname);
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
    currentLogfileName = getFilename(logfolder_taskmapper);
    logfileTaskmapper.open(currentLogfileName);
}

void closeLogFileTaskmapper() {
    logfileTaskmapper.close();
}

/**
 * Für die Abbildung 1:N werden die Tasks parallel geprüft. Aus diesem Grund werden die Zwischenergebnisse hier zusammengeführt.
 */
Result resultOneToMany;

void logMessageOnTaskmapperFileAndCout(std::string text, bool withLogFileTaskmapper){
    std::cout << text;
    if (withLogFileTaskmapper) {
        logfileTaskmapper << text;
    }
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

void saveOneToOneLine(std::string newOneToOneLine) {
    std::ifstream infile(filename_taskmap_result);

    size_t delimiterPos = newOneToOneLine.find('=');
    std::string newKey = newOneToOneLine.substr(0, delimiterPos);
    std::string newValue = newOneToOneLine.substr(delimiterPos + 1);
    bool newKeyfound = false;

    std::list<std::string> logs;
    std::list<std::string> oneToOne;
    std::list<std::string> manyToOne;

    std::string line;
    std::string currentSection;
    while (std::getline(infile, line)) {
        if (line.empty()) {
            continue;
        }
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }

        size_t delimiterPos = line.find('=');

        if (currentSection==result_section_log) {
            logs.push_back(line);
        } else if (currentSection==result_section_one2one) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            if (newKey==key) {
                newKeyfound=true;
                oneToOne.push_back(newOneToOneLine);
            } else {
                oneToOne.push_back(line);
            }
        } else if (currentSection==result_section_many2one) {
            manyToOne.push_back(line);
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
    if (!newKeyfound) {
        oufile << newOneToOneLine<<"\n";
    }

    oufile << "\n";
    oufile << "[" << result_section_many2one << "]\n";
    for (std::string l : manyToOne) {
        oufile << l << "\n";
    }

    oufile.close();
}

/**
 * Wird nach Ausführung der Ähnlichkeitssuche eines Anwendungstasks ausgeführt. Zu dem übergebenen Anwendungstask soll der Prototyptask ausgegeben werden, der dem Anwendungstask am ähnlichsten ist.
 *
 * @param appTask für diesen Anwendungstask wird das Ergebnis geprüft.
 */
AnwTask logBestTask(AnwTask appTask, bool percent) {
    logMessageOnTaskmapperFileAndCout(
            "Der ähnlichste ProttypTask für den AppTask " + appTask.name + " ist " + appTask.bestName + "(" +
            std::to_string(appTask.besthit) + " " + (percent ? "%" : "") + " Treffer)\n\n", true);
    saveOneToOneLine(appTask.name + "=" + appTask.bestName);
    return appTask;
}
