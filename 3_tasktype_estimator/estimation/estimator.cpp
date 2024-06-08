#include "../include/estimator.h"
#include <sys/stat.h> //mkdir, chmod
#include <fstream>
#include <string>
std::ofstream resultFile;

void openResultFile() {
    std::string filename = getFilename(folder_result, logfileres);
    resultFile.open(filename);
    chmod(filename.c_str(), 0777);
}

void closeResultFile() {
    resultFile.close();
}


void logHeadline() {
    resultFile << "CPUFrequency;Parallelism;apptask;apptaskduration;apptaskpower:\testimationOneToOne;diffOneToOne;\testimationOneToMany;diffOneToMany\n";
}

void logEmptyline() {
    resultFile << "\n";
}

void startApptaskEstimation(std::string apptaskname) {
    MeasureResult result = estimateAppTask(apptaskname, currentCPUFreq, currentParallelism);
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

void repeatEstimationsForAppTask(std::string apptaskname, int repeats) {

    for (std::string cpuFreq : cpuFrequencyVektor) {
        setupCpuFrequenzlevel(cpuFreq);

        for (std::string cores : parallelismVektor) {
            setCurrentParallelism(cores);

            for (int i = 0; i < repeats; i++) {
                startApptaskEstimation(apptaskname);
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
        repeatEstimationsForAppTask(apptaskname, repeats);
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

    repeatEstimationsForAppTask(apptaskname, repeats);

    closeMeasureLogFile();
    closeResultFile();
    smton();
}