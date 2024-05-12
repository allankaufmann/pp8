#include <sys/stat.h> //mkdir, chmod
std::ofstream resultFile;

void openResultFile() {
    std::string filename = getFilename(folder_result, logfileres);
    resultFile.open(filename);
    chmod(filename.c_str(), 0777);
}

void closeResultFile() {
    resultFile.close();
}

std::string readOneToOneMapping(std::string apptaskname) {
    std::ifstream infile(filename_taskmap_result_from_folder);
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
    infile.close();
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
    logMeasureNewLine();
    logMeasureFlush();

    resultFile << currentCPUFreq << ";";
    resultFile << currentParallelism << ";";
    resultFile << apptaskname << ";";
    resultFile << appTaskresult.duration << " MS; ";
    resultFile << appTaskresult.power() << " MJ/MS:\t";
    resultFile << result.power() << " MJ/MS; ";
    if (appTaskresult.power()!=0) {
        resultFile << (result.power()-appTaskresult.power())*100/appTaskresult.power() << "%;\t";
        resultFile << result.powerOneToMany() << " MJ/MS;";
        resultFile << (result.powerOneToMany()-appTaskresult.power())*100/appTaskresult.power() << "%;";
        resultFile << "\n";
    } else {
        resultFile << 0 << "%;\t";
        resultFile << result.powerOneToMany() << " MJ/MS;";
        resultFile << 0 << "%;\t";
        resultFile << "\n";
    }
    resultFile.flush();
}



void estimateAppTask(std::string apptaskname, int repeats) {

    for (std::string cpuFreq : cpuFrequencyVektor) {
        setupCpuFrequenzlevel(cpuFreq);

        for (std::string cores : parallelismVektor) {
            setCurrentParallelism(cores);

            for (int i = 0; i < repeats; i++) {
                estimateApptask(apptaskname);
            }
            logEmptyline();
        }
    }
}


void startEstimation(int repeats) {
    smtoff();
    openResultFile();
    logHeadline();
    openMeasurLogFile();


    for (std::string apptaskname: apptypeVektor) {
        estimateAppTask(apptaskname, repeats);
    }

    closeMeasureLogFile();
    closeResultFile();
    smton();
}

void testEstimation(std::string apptaskname, int repeats) {
    smtoff();
    openResultFile();
    logHeadline();
    openMeasurLogFile();

    estimateAppTask(apptaskname, repeats);

    closeMeasureLogFile();
    closeResultFile();
    smton();
}