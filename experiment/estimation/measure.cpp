//
// Created by allan on 27.01.24.
//
#include <stdio.h>
#include <sys/stat.h> //mkdir
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include "vector"
#include "../constants.h"
#include "../tools.cpp"
#include <vector>
#include <fstream>
#include <unistd.h>
static const char *const logfolder_measure = "logs/measure/";

class MeasureResult {
public:

    std::string taskname;
    long long  duration;
    long long energy_mj;
    std::string cpuFreq;
    std::string parallelism;

    long long duration_one_to_many;
    long long energy_my_one_to_many;

    float power() {
        return energy_mj/duration;
    }

    float powerOneToMany() {
        return energy_my_one_to_many/duration_one_to_many;
    }


};

FILE* logfileMeasure;


// https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void logTime() {
    std::time_t result = std::time(nullptr);
    std::cout << std::ctime(&result);
}

long long readCounterFromFile() {
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[bufferLength];

    filePointer = fopen("logs/counter.txt", "r");

    //printf("/sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj:\n");

    long long counter = -1;

    while(fgets(buffer, bufferLength, filePointer)) {
        counter = strtol(buffer, NULL, 10);
    }

    fclose(filePointer);



    return counter;
}

long long  readEnergy_UJ_script_deprecated() {
    system("./scripts/readEnergy_UJ.sh");
    return readCounterFromFile();
}



long unsigned  readEnergy_UJ() {
    FILE *filePointer;
    filePointer = popen("cat /sys/class/powercap/intel-rapl/intel-rapl\\:0/energy_uj", "r");

    long unsigned energy_ui=0;

    fscanf(filePointer, "%lu", &energy_ui);
    //printf("%lu\n", energy_ui);

    pclose(filePointer);
    return energy_ui;
}

long unsigned  readEnergy_UJ_better_with_loop() {
    long unsigned energy_ui = readEnergy_UJ();
    long unsigned new_energy_ui = readEnergy_UJ();
    while (new_energy_ui == energy_ui) {
        new_energy_ui = readEnergy_UJ();
    }
    return new_energy_ui;
}


long unsigned  measureIdle(int milliseconds) {
    long unsigned  counter_beginn = readEnergy_UJ();
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    long unsigned counter_end = readEnergy_UJ();
    return counter_end - counter_beginn;
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
    logfileMeasure = fopen(getFilename(), "w");
    fprintf(logfileMeasure, "app;duration;power;leistung\n");
}

void closeMeasureLogFile() {
    fclose(logfileMeasure);
}

void logMeasure(const char app[], long long  dauer, long long energy_mj) {
    fprintf(logfileMeasure, "%s;%lld;%lld;%lld\n", app, dauer, energy_mj, energy_mj/dauer);
}

void logMeasureNewLine() {
    fprintf(logfileMeasure, "\n");
}




void runCommand(const char* command) {
    std::cout << "Skript " << command << " wird gestartet" << std::endl;
    system(command);
}

MeasureResult runAndMeasureScript(const char* script) {
    uint64_t timestamp_begin = timeSinceEpochMillisec();
    long long counter_begin = readEnergy_UJ_better_with_loop();
    std::thread t1(runCommand, script);
    t1.join();
    long long counter_end = readEnergy_UJ();
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

char* searchTasktypeFile(std::string tasktypename, std::string folder) {
    std::vector<char*> v_filenames = readFilenamesFromDirectory(folder.c_str());

    for (char* filename : v_filenames) {
        char c_taskname[strlen(filename)];
        extractTaskNameFromFileName(filename, c_taskname);
        if (strcmp(c_taskname, tasktypename.c_str())==0) {
            return filename;
        }
    }
    return NULL;
}

std::string  getFilenameWithParam(char* filename, std::string cores) {
    std::string fileNameWithParam = filename;
    fileNameWithParam+= " " + cores;
    return fileNameWithParam;
}

MeasureResult estimateAppTask(std::string apptaskname, std::string taskname, std::string cpufreq, std::string cores) {
    char* filename = searchTasktypeFile(taskname, foldername_generated_scripts_tasktypes_from_folder);



    std::string fileWithParam = getFilenameWithParam(filename, cores);

    MeasureResult result = runAndMeasureScript(fileWithParam.c_str());
    result.taskname=taskname;
    result.cpuFreq=cpufreq;
    result.parallelism=cores;

    char* filenameOneToMany = searchTasktypeFile(apptaskname, foldername_generated_scripts_tasktypes_onetomany);
    std::string filenameOneToManyWithParam = getFilenameWithParam(filenameOneToMany, cores);


    MeasureResult resultOneToMany = runAndMeasureScript(filenameOneToManyWithParam.c_str());
    result.duration_one_to_many = resultOneToMany.duration;
    result.energy_my_one_to_many = resultOneToMany.energy_mj;

    logMeasureNewLine();

    return result;
}

MeasureResult measureAppTask(std::string apptaskname, std::string cpufreq, std::string cores) {
    char* filename = searchTasktypeFile(apptaskname, foldername_generated_scripts_apptasks);

    std::string fileWithParam = getFilenameWithParam(filename, cores);

    std::thread t1(runCommand, fileWithParam.c_str());
    t1.join();

    char* measureFilename = searchTasktypeFile(apptaskname, folder_measure);

    std::ifstream measureFile(measureFilename);
    std::string line;

    MeasureResult result;
    while (std::getline(measureFile, line)) {
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
        //std::cout << line << "\n";
    }

    logMeasure(apptaskname.c_str(), result.duration, result.energy_mj);
    logMeasureNewLine();
    return result;
}



