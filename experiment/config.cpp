#include <iostream>
#include <string.h> //strtok
#include <sys/stat.h> //mkdir
#include <fstream>
#include "constants.h"

std::string section_tasktypes="TaskTypes";

std::string section_apptasks="ApplicationTasks";

std::string section_cpufrequency="CPUFrequency";

std::string section_parallelism="Parallelism";

std::string currentCPUFreq;

std::vector<std::string> tasktypeVektor;

std::vector<std::string> apptypeVektor;

std::vector<std::string> cpuFrequencyVektor;

std::vector<std::string> parallelismVektor;

void writeGenScript(const char* task) {
    FILE* filePointerScript;
    //char* filename = (char*) calloc(40, sizeof(char));
    char* filename = (char*) malloc(sizeof(char) * (strlen(task)) + 10);
    sprintf(filename,
            "%s%s%s",
            "gen/"
            "run",
            task,
            ".sh");

    filePointerScript = fopen(filename, "w");


    fprintf(filePointerScript, "%s", "#!/bin/bash\n");
    fprintf(filePointerScript, "%s", "cd ..\n");
    fprintf(filePointerScript, "%s", "cd epEBench/bin/Release\n");
    fprintf(filePointerScript, "%s%s%s", "./epebench -m ", task, " -t 1 -a 1 -n 1\n");
    fprintf(filePointerScript, "%s%s%s", "mv epebench_loadlog.txt epebench_", task, ".log\n");
    fprintf(filePointerScript, "%s", "cd ../../..\n");


    fclose(filePointerScript);
    chmod(filename, 0777);
    free(filename);
}

void readConfigFile() {
    mkdir(foldername_generated_scripts, 0777);

    std::ifstream file(config_filename2);
    std::string currentSection;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }

        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos)
            continue;

        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        if (currentSection==section_tasktypes) {
            tasktypeVektor.push_back(value);
        } else if (currentSection==section_apptasks) {
            apptypeVektor.push_back(value);
        } else if (currentSection==section_cpufrequency) {
            cpuFrequencyVektor.push_back(value);
        } else if (currentSection==section_parallelism) {
            parallelismVektor.push_back(value);
        }
    }

    for (std::string s : tasktypeVektor) {
        std::cout << s << "\n";
        writeGenScript(s.c_str());
    }

    printf("%s", "Skripte zum Ausführen der prototypischen Tasks wurden im Ordner 'gen' erstellt!");

    for (std::string s : apptypeVektor) {
        std::cout << s << "\n";
    }

    for (std::string s : cpuFrequencyVektor) {
        std::cout << s << "\n";
    }

    for (std::string s : parallelismVektor) {
        std::cout << s << "\n";
    }
}

void setCpuFrequency() {
    if (cpuFrequencyVektor.size()==0) {
        readConfigFile();
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

