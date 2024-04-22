#include <sys/stat.h> //mkdir, chmod



std::ofstream resultFile;

void openResultFile() {
    resultFile.open(filename_estimation_result);
    chmod(filename_estimation_result.c_str(), 0777);
}

void closeResultFile() {
    resultFile.close();
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
    resultFile << "CPUFrequency;Parallelism;apptask;apptaskduration;apptaskpower:\testimationOneToOne;diffOneToOne;\testimationOneToMany;diffOneToMany\n";
}

void logEmptyline() {
    resultFile << "\n";
}

void estimateApptask(std::string apptaskname) {
    std::string oneToOneTask = readOneToOneMapping(apptaskname);

    MeasureResult result = estimateAppTask(apptaskname, oneToOneTask, currentCPUFreq, currentParallelism);

    MeasureResult appTaskresult = measureAppTask(apptaskname, currentCPUFreq, currentParallelism);

    resultFile << currentCPUFreq << ";";
    resultFile << currentParallelism << ";";
    resultFile << apptaskname << ";";
    resultFile << appTaskresult.duration << " MS; ";
    resultFile << appTaskresult.power() << " MJ/MS:\t";
    resultFile << result.power() << " MJ/MS; ";
    resultFile << (result.power()-appTaskresult.power())*100/appTaskresult.power() << "%;\t";
    resultFile << result.powerOneToMany() << " MJ/MS;";
    resultFile << (result.powerOneToMany()-appTaskresult.power())*100/appTaskresult.power() << "%;";
    resultFile << "\n";
}

void testEstimation() {
    openResultFile();
    logHeadline();
    openMeasurLogFile();

    setupCpuFrequenzlevel(cpuFrequencyVektor[5]);

    setCurrentParallelism(parallelismVektor[0]);
    for (std::string apptaskname: apptypeVektor) {
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        logEmptyline();
    }

    logEmptyline();
    setCurrentParallelism(parallelismVektor[1]);
    for (std::string apptaskname: apptypeVektor) {
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        logEmptyline();
    }

    logEmptyline();
    setCurrentParallelism(parallelismVektor[2]);
    for (std::string apptaskname: apptypeVektor) {
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        logEmptyline();
    }

    setupCpuFrequenzlevel(cpuFrequencyVektor[4]);

    setCurrentParallelism(parallelismVektor[0]);
    logEmptyline();
    for (std::string apptaskname: apptypeVektor) {
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        logEmptyline();
    }
    setCurrentParallelism(parallelismVektor[1]);
    for (std::string apptaskname: apptypeVektor) {
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        logEmptyline();
    }
    setCurrentParallelism(parallelismVektor[2]);
    for (std::string apptaskname: apptypeVektor) {
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        estimateApptask(apptaskname);
        logEmptyline();
    }



    closeMeasureLogFile();
    closeResultFile();
}