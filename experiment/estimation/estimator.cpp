std::ofstream resultFile;
std::string currentCPUFreq;
std::string currentParallelism;

void openResultFile() {
    resultFile.open(filename_estimation_result);
}

void closeResultFile() {
    resultFile.close();
}



void selectCpuFrequency() {
    if (cpuFrequencyVektor.size()==0) {
        readConfigFile(false, false);
    }

    printf("In der Konfigurationsdatei experiment.config sind %d CPU-Level hinterlegt, bitte durch Eingabe auswählen!\n", cpuFrequencyVektor.size());

    for (int i = 0; i < cpuFrequencyVektor.size(); i++) {
        printf("[%d]: %s\n", i, cpuFrequencyVektor[i].c_str());
    }

    int index = 0;
    if (scanf("%d", &index) == 1) {
        currentCPUFreq = cpuFrequencyVektor[index];
        std::string frequence = "echo allan | sudo -S cpupower frequency-set -u " + currentCPUFreq + "mhz";
        system(frequence.c_str());
        system("cpupower frequency-info");
    }
}

void setupCpuFrequenzlevel(std::string frequenz) {
    currentCPUFreq = frequenz;
    std::string frequence = "echo allan | sudo -S cpupower frequency-set -u " + currentCPUFreq + "mhz";
    system(frequence.c_str());
    system("cpupower frequency-info");
    printf("CPU Frequenz wurde auf %s MHz eingestellt!\n", frequenz.c_str());
}

void setCurrentParallelism(std::string parallel) {
    currentParallelism = parallel;
}

std::string readOneToOneMapping(std::string apptaskname) {
    std::ifstream infile(filename_taskmap_result);
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
        if (currentSection==result_section_one2one) {
            size_t delimiterPos = line.find('=');
            std::string keyOfCurrentLine = (delimiterPos != 0) ? line.substr(0, delimiterPos) : NULL;
            std::string valueOfCurrentLine = (delimiterPos != 0) ? line.substr(delimiterPos + 1) : NULL;
            if (keyOfCurrentLine == apptaskname) {
                return valueOfCurrentLine;
            }
        }
    }

    return NULL;
}

void logHeadline() {
    resultFile << "CPUFrequency;apptask;Parallelism;oneToOneTask;estimationOneToOne;estimationOneToMany;apptaskduration;measure;diffOneToOne;diffOneToMany\n";
}

void estimateApptask(std::string apptaskname) {
    std::string oneToOneTask = readOneToOneMapping(apptaskname);

    MeasureResult result = estimateAppTask(apptaskname, oneToOneTask, currentCPUFreq, currentParallelism);


    MeasureResult appTaskresult = measureAppTask(apptaskname, currentCPUFreq, currentParallelism);

    resultFile << currentCPUFreq << ";";
    resultFile << apptaskname << ";";
    resultFile << currentParallelism << ";";
    resultFile << oneToOneTask << ";";
    resultFile << result.power() << ";";
    resultFile << result.powerOneToMany() << ";";
    resultFile << appTaskresult.duration << ";";
    resultFile << appTaskresult.power() << ";";
    resultFile << (result.power()-appTaskresult.power())*100/appTaskresult.power() << "%;";
    resultFile << (result.powerOneToMany()-appTaskresult.power())*100/appTaskresult.power() << "%;";
    resultFile << "\n";
}

void testEstimation() {
    openResultFile();
    logHeadline();
    setupCpuFrequenzlevel(cpuFrequencyVektor[5]);
    setCurrentParallelism(parallelismVektor[0]);

    openMeasurLogFile();

    for (std::string apptaskname: apptypeVektor) {
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
    }



    closeMeasureLogFile();
    closeResultFile();
}