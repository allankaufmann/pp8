#include "../include/measure.h"
#include <stdio.h>
#include <sys/stat.h> //mkdir
#include <dirent.h>
#include "vector"
#include "../include/constants.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include "../include/config.h"
#include "../include/tools.hpp"
#include <thread>
#include <cstring>
static const char *const logfolder_measure = "logs/measure/";

std::ofstream logfileMeasure;
std::string currentCPUFreq;
std::string currentParallelism;

// https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

char* getFilename() {
    time_t mytime = time(NULL);
    tm local_tm = *localtime(&mytime);

    int year = local_tm.tm_year + 1900;
    int month = local_tm.tm_mon+1;

    char* filename = (char*) malloc(sizeof(char) * 30);
    sprintf(filename, "%s%d%s%d%s%d%s%d%s%d%s%d.log",
            logfolder_measure,
            year,
            (month<10) ? "0" : "",
            month,
            (local_tm.tm_mday<10) ? "0" : "",
            local_tm.tm_mday,
            (local_tm.tm_hour<10) ? "0" : "",
            local_tm.tm_hour,
            (local_tm.tm_min<10) ? "0" : "",
            local_tm.tm_min,
            (local_tm.tm_sec<10) ? "0" : "",
            local_tm.tm_sec
            );

    return filename;
}

void openMeasurLogFile() {
    mkdir(logfolder_measure, 0777);
    logfileMeasure.open(getFilename());
    logfileMeasure << "app;cpufreq;Parallelitätsgrad;Dauer; power;leistung\n";
}

void closeMeasureLogFile() {
    logfileMeasure.close();
}

void logMeasure(const char app[], long long  dauer, long long energy_mj) {
    logfileMeasure << app << ";" << dauer << ";" << energy_mj << ";" << ((dauer!=0) ? energy_mj/dauer : 0) << "\n";
}

void logMeasure(MeasureResult res) {
    logfileMeasure << res.taskname.c_str() << ";" << res.cpuFreq.c_str() << ";" << res.parallelism.c_str() << ";" <<  res.duration << ";" << res.energy_mj << ";" << res.power() << "\n";
}

void logMeasureNewLine() {
    logfileMeasure << "\n";
}

void logMeasureFlush() {
    logfileMeasure.flush();
}

void runCommand(const char* command) {
    std::cout << "Skript " << command << " wird gestartet" << std::endl;
    system(command);
}

MeasureResult runAndMeasureScript(const char* script) {
    uint64_t timestamp_begin = timeSinceEpochMillisec();
    long long counter_begin = readEnergy_UJ_secure();
    std::thread t1(runCommand, script);
    t1.join();
    long long counter_end = readEnergy_UJ_secure();
    uint64_t timestamp_end = timeSinceEpochMillisec();
    uint64_t duration = timestamp_end - timestamp_begin;
    std::cout << "Dauer: " << duration << " MS" << std::endl;
    long long counter_diff = counter_end - counter_begin;
    long long energy_mj = counter_diff;
    std::cout << "Leistungsaufnahme in Mikojoul: " << (energy_mj) << std::endl;

    MeasureResult result;
    result.duration=duration;
    result.energy_mj=energy_mj;

    logMeasure(script, duration, energy_mj);
    logMeasureFlush();
    return result;
}

void runAndMeasureScriptsFromDirectory(int count, const char* directory, const char* cores) {
    std::vector<char*> v_filenames = readFilenamesFromDirectory(directory);
    openMeasurLogFile();
        for (char* filename : v_filenames) {

            char fileWithParam[strlen(filename)+2];
            strcpy(fileWithParam, filename);
            strcat(fileWithParam, " ");// Test mit 2 Cores

            strcat(fileWithParam, cores);
            for (int i = 0; i<count; i++) {

                runAndMeasureScript(fileWithParam);
            }
            logMeasureNewLine();
        }
    closeMeasureLogFile();

    printf("%s", "Skripte wurden ausgeführt, Ergebnisse siehe logs-Ordner!");
}


std::string  getFilenameWithParam(char* filename, std::string cores) {
    std::string fileNameWithParam = filename;
    fileNameWithParam+= " " + cores;
    return fileNameWithParam;
}

MeasureResult estimateAppTask(std::string apptaskname, std::string cpufreq, std::string cores) {
    std::string oneToOneTaskname = readOneToOneMapping(apptaskname);
    char* filenameOneToOne = searchTasktypeFile(oneToOneTaskname, foldername_generated_scripts_tasktypes_from_folder);

    std::string filenameOneToOneWithParam = getFilenameWithParam(filenameOneToOne, cores);

    MeasureResult result = runAndMeasureScript(filenameOneToOneWithParam.c_str());
    result.taskname=oneToOneTaskname;
    result.cpuFreq=cpufreq;
    result.parallelism=cores;

    char* filenameOneToMany = searchTasktypeFile(apptaskname, foldername_generated_scripts_tasktypes_onetomany);
    std::string filenameOneToManyWithParam = getFilenameWithParam(filenameOneToMany, cores);

    MeasureResult resultOneToMany = runAndMeasureScript(filenameOneToManyWithParam.c_str());
    result.duration_one_to_many = resultOneToMany.duration;
    result.energy_my_one_to_many = resultOneToMany.energy_mj;
    logMeasureFlush();

    return result;
}

MeasureResult measureAppTask(std::string apptaskname, std::string cpufreq, std::string cores) {
    char* filename = searchTasktypeFile(apptaskname, foldername_generated_scripts_apptasks);

    std::string fileWithParam = getFilenameWithParam(filename, cores);

    std::thread t1(runCommand, fileWithParam.c_str());
    t1.join();
    char* measureFilename = searchTasktypeFile(apptaskname, folder_measure);

    MeasureResult result;
    result.parallelism=-1;
    result.duration=-1;
    result.energy_mj=-1;
    result.taskname=apptaskname;
    result.cpuFreq=cpufreq;

    if (measureFilename!=NULL) {
        std::ifstream measureAppTaskFile(measureFilename);
        std::string line;

        while (std::getline(measureAppTaskFile, line)) {
            if (line.empty()) {
                continue;
            }

            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) {
                continue;
            }

            std::string keyOfCurrentLine = (delimiterPos != 0) ? line.substr(0, delimiterPos) : NULL;
            std::string valueOfCurrentLine = (delimiterPos != 0) ? line.substr(delimiterPos + 1) : NULL;

            if (keyOfCurrentLine=="parallelism") {
                result.parallelism=valueOfCurrentLine;
            } else if (keyOfCurrentLine=="duration") {
                result.duration=stoi(valueOfCurrentLine);
            } else if (keyOfCurrentLine=="counter_diff") {
                result.energy_mj=stoll(valueOfCurrentLine);
            }
        }
        measureAppTaskFile.close();
        logMeasure(apptaskname.c_str(), result.duration, result.energy_mj);
        logMeasureNewLine();
    }

    return result;
}


void selectCpuFrequency() {
    if (cpuFrequencyVektor.size()==0) {
        readConfigFile(false, false);
    }

    printf("In der Konfigurationsdatei experiment.config sind %lu CPU-Level hinterlegt, bitte durch Eingabe auswählen!\n", cpuFrequencyVektor.size());

    for (int i = 0; i < cpuFrequencyVektor.size(); i++) {
        printf("[%d]: %s\n", i, cpuFrequencyVektor[i].c_str());
    }

    int index = 0;
    if (scanf("%d", &index) == 1) {
        currentCPUFreq = cpuFrequencyVektor[index];
        std::string frequence = "cpupower frequency-set -u " + currentCPUFreq + "mhz";
        system(frequence.c_str());
        system("cpupower frequency-info");
    }
}

void setupCpuFrequenzlevel(std::string frequenz) {
    currentCPUFreq = frequenz;
    std::string frequence = "sudo -S cpupower frequency-set -u " + currentCPUFreq + "mhz";
    system(frequence.c_str());
    system("cpupower frequency-info");
    printf("CPU Frequenz wurde auf %s MHz eingestellt!\n", frequenz.c_str());
}

void setCurrentParallelism(std::string parallel) {
    currentParallelism = parallel;
}

void smtoff() {
    system("echo off > /sys/devices/system/cpu/smt/control");
}

void smton() {
    system("echo on > /sys/devices/system/cpu/smt/control");
}

void measureAllPrototypTasks(int count) {
    readConfigFile(false, false);
    openMeasurLogFile();

    std::string cpuFrequency = cpuFrequencyVektor[5];

        for (std::string parallelism : parallelismVektor) {
            for(std::string tasktype: tasktypeVektor) {
                for (int i = 0; i < count; i++) {
                    char* filename = searchTasktypeFile(tasktype, foldername_generated_scripts_tasktypes);
                    std::string fileWithParam = getFilenameWithParam(filename, parallelism);
                    MeasureResult result = runAndMeasureScript(fileWithParam.c_str());
                    result.taskname=tasktype;
                    result.cpuFreq=cpuFrequency;
                    result.parallelism=parallelism;
                    logMeasure(result);
                }
                logMeasureNewLine();
            }
        }

    closeMeasureLogFile();
}



