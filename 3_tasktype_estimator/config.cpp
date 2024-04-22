#include <iostream>
#include <string.h> //strtok
#include <sys/stat.h> //mkdir, chmod
#include <fstream>
#include "constants.h"
#include <list>
#include "vector"
std::string section_tasktypes="TaskTypes";

std::string section_apptasks="ApplicationTasks";

std::string section_cpufrequency="CPUFrequency";

std::string section_parallelism="Parallelism";

std::vector<std::string> tasktypeVektor;

std::vector<std::string> apptypeVektor;

std::vector<std::string> cpuFrequencyVektor;

std::vector<std::string> parallelismVektor;

void writeGenScript(const char* task) {
    FILE* filePointerScript;
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

void generateTaskTypeScripts(std::string folder, std::string task) {
    // $CORES ist der Parameter für Anzahl Kerne!
    std::string filename = folder + "/" + task + ".sh";
    std::ofstream scriptfile (filename);
    if (scriptfile.is_open()) {
        scriptfile << "#!/bin/bash\n";
        scriptfile << "CORES=$1\n";
        scriptfile << "if [ -z \"$CORES\" ]\n";
        scriptfile << "then\n";
        scriptfile << "CORES=1\n";
        scriptfile << "fi\n";
        scriptfile << "echo Anzahl ist $CORES\n";
        scriptfile << "cd ..\n";
        scriptfile << "cd epEBench/bin/Release\n";
        scriptfile << "./epebench -m " + task + " -t 5 -n $CORES -u 100 \n";
        scriptfile << "chmod a+w epebench_loadlog.txt\n";
        scriptfile << "mv epebench_loadlog.txt epebench_" + task + ".log\n";
        scriptfile << "cd ../../..";
    }
    scriptfile.close();
    chmod(filename.c_str(), 0777);
}

void generateAppTaskScripts(std::string task) {
    // $CORES ist der Parameter für Anzahl Kerne!
    std::string filename = foldername_generated_scripts_apptasks + "/" + task + ".sh";
    std::ofstream scriptfile (filename);
    if (scriptfile.is_open()) {
        scriptfile << "#!/bin/bash\n";
        scriptfile << "CORES=$1\n";
        scriptfile << "if [ -z \"$CORES\" ]\n";
        scriptfile << "then\n";
        scriptfile << "CORES=1\n";
        scriptfile << "fi\n";
        scriptfile << "echo Anzahl ist $CORES\n";
        scriptfile << "cd ..\n";
        scriptfile << "cd edgedetection\n";
        scriptfile << "echo \"$PWD\"\n";
        scriptfile << "./edgedetection imgfilenames $CORES " + task + "\n";
    }
    scriptfile.close();
    chmod(filename.c_str(), 0777);
}

void readConfigFile(bool showEntries, bool generateScripts) {
    mkdir(foldername_generated_scripts, 0777);
    mkdir(foldername_generated_scripts_tasktypes.c_str(), 0777);
    mkdir(foldername_generated_scripts_apptasks.c_str(), 0777);

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

    if (showEntries) {
        std::cout << "Folgende Tasktypen sind konfiguriert:\n";
    }

    for (std::string s : tasktypeVektor) {
        if (showEntries) {
            std::cout << "\t" << s << "\n";
        }
        if (generateScripts) {
            generateTaskTypeScripts(foldername_generated_scripts_tasktypes, s);
        }
    }

    // Für one2Many-Abbildung werden weitere Skripte für die Tasktypen benötigt!
    if (generateScripts) {
        for (std::string s: apptypeVektor) {
            generateTaskTypeScripts(foldername_generated_scripts_tasktypes_onetomany, s);
        }
    }




    if (showEntries) {
        std::cout << "\nFolgende Anwendungstasks sind konfiguriert:\n";
    }

    for (std::string s: apptypeVektor) {
        if (showEntries) {
            std::cout << "\t" << s << "\n";
        }

        if (generateScripts) {
            generateAppTaskScripts(s);
        }
    }

    if (generateScripts) {
        std::cout  << "\nSkripte zum Ausführen der Tasks wurden im Ordner 'gen' erstellt!\n";
    }

    if (showEntries) {
        std::cout << "\nFolgende CPU Frequenzlevel sind hinterlegt:\n";
        for (std::string s : cpuFrequencyVektor) {
            std::cout << "\t" << s << "\n";
        }

        std::cout << "\nFolgende Parallelitätslevel sind hinterlegt:\n";
        for (std::string s : parallelismVektor) {
            std::cout << "\t" << s << "\n";
        }

    }
}

