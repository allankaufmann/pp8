#include <iostream>
#include <string.h> //strtok
#include <sys/stat.h> //mkdir, chmod
#include <fstream>
#include "constants.h"
#include <list>
#include "vector"
#include "tools.cpp"

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

    std::ifstream file(config_filename);
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

long unsigned  readEnergy_UJ() {
    FILE *filePointer;
    filePointer = popen("cat /sys/class/powercap/intel-rapl/intel-rapl\\:0/energy_uj", "r");

    long unsigned energy_ui=0;

    fscanf(filePointer, "%lu", &energy_ui);

    int status = pclose(filePointer);
    if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        if (exit_status == 1) {
            std::cout << RED << "Fehler beim Lesen von intel-rapl/energy_uj! Root-User? "<< RESET;
        }
    }

    return energy_ui;
}

void checkPreconditions() {
    std::ifstream file(config_filename);

    if (file.is_open()) {
        std::cout << "Die Konfigurationsdatei " << config_filename << " ist vorhanden!\n";
    } else {
        std::cout << RED << "Die Konfigurationsdatei " << config_filename << " ist nicht vorhanden!" << RESET << "\n";
    }

    readConfigFile(false, false);

    std::cout << "---------------------------------------------------------------------------------------------------------------" << "\n";
    std::cout << "Überprüfung generierter Skripte:" << "\n";
    std::cout << "\tÜberprüfung: Ordner gen/tasktypes:" << "\n";
    for (std::string s: tasktypeVektor) {
        char* filename = searchTasktypeFile(s, foldername_generated_scripts_tasktypes);

        if (filename==NULL) {
            std::cout << RED << "\tFür den Tasktyp " << s << " wurde keine Datei gefunden! " << config_filename << " überprüfen und Config ausführen!" << RESET << "\n";
        } else {
            std::cout << "\tFür den Tasktypen " << s << " ist die Datei " << filename << " vorhanden\n";
        }
    }

    std::cout << "\n\tÜberprüfung: Ordner gen/tasktypes/onetomany:" << "\n";
    for (std::string s: apptypeVektor) {
        char* filename = searchTasktypeFile(s, foldername_generated_scripts_tasktypes_onetomany);

        if (filename==NULL) {
            std::cout << RED << "\tFür den Tasktyp " << s << " ist im Ordner " << foldername_generated_scripts_tasktypes_onetomany << " keine Datei vorhanden! " << config_filename << " überprüfen und Config ausführen!" << RESET << "\n";
        } else {
            std::cout << "\tFür den Apptasks " << s << " ist im Ordner " << foldername_generated_scripts_tasktypes_onetomany << " die Datei " << filename << " vorhanden\n";
        }
    }

    std::cout << "\nÜberprüfung: Ordner gen/apptasks:" << "\n";
    for (std::string s: apptypeVektor) {
        char* filename = searchTasktypeFile(s, foldername_generated_scripts_apptasks);
        if (filename==NULL) {
            std::cout << RED << "\tFür den Apptask " << s << " wurde keine Datei gefunden! " << config_filename << " überprüfen und Config ausführen!" << RESET << "\n";
        } else {
            std::cout << "\tFür den Apptasks " << s << " ist die Datei " << filename << " vorhanden\n";
        }

    }

    std::cout << "---------------------------------------------------------------------------------------------------------------" << "\n";
    std::cout << "Überprüfung Taskmapper:" << "\n";
    std::cout << "\nÜberprüfung: Ordner " << foldername_appseq << "\n";
    for (std::string s: apptypeVektor) {
        char* filename = searchTasktypeFile(s, foldername_appseq);
        if (filename==NULL) {
            std::cout << RED << "\tFür den Apptask " << s << " wurde keine Datei im Ordner " << foldername_appseq << " gefunden! Anwendung 2_apptask_analyzer ausführen!" << RESET << "\n";
        } else {
            std::cout << "\tFür den Apptasks " << s << " ist die Datei " << filename << " vorhanden\n";
        }
    }
    std::cout << "\nÜberprüfung: Ordner " << foldername_seq << "\n";
    for (std::string s: tasktypeVektor) {
        char* filename = searchTasktypeFile(s, foldername_seq);
        if (filename==NULL) {
            std::cout << RED << "\tFür den Tasktyp " << s << " wurde keine Datei im Ordner " << foldername_seq << " gefunden! Anwendung 1_tasktype_analyzer ausführen!" << RESET << "\n";
        } else {
            std::cout << "\tFür den Tasktyp " << s << " ist die Datei " << filename << " vorhanden\n";
        }
    }

    std::cout << "\nÜberprüfung Taskmapper Ergebnisdatei " << filename_taskmap_result << "\n";
    for (std::string s: apptypeVektor) {
        std::ifstream infile(filename_taskmap_result_from_folder);
        std::string line;
        std::string currentSection;
        std::string currentModel;
        bool one2one = false;
        bool one2many = false;
        while (std::getline(infile, line)) {
            if (line[0] == '[' && line[line.length() - 1] == ']') {
                currentSection = line.substr(1, line.length() - 2);
                continue;
            }
            if (currentSection==result_section_one2one) {
                size_t delimiterPos = line.find('=');
                std::string keyOfCurrentLine = (delimiterPos != 0) ? line.substr(0, delimiterPos) : NULL;
                if (keyOfCurrentLine==s) {
                    one2one=true;
                }
            }
            if (currentSection==result_section_one2many) {
                size_t delimiterPos = line.find('=');
                std::string keyOfCurrentLine = (delimiterPos != 0) ? line.substr(0, delimiterPos) : NULL;
                std::string valueInLine = (delimiterPos!=0) ? line.substr(delimiterPos + 1):NULL;
                if (keyOfCurrentLine == result_section_model) {
                    currentModel = valueInLine;
                }
                if (currentModel == s) {
                    one2many=true;
                }
            }

        }

        if (one2one) {
            std::cout << "\tFür den appTask " << s << " ist eine 1:1-Abbildung vorhanden.\n";
        } else {
            std::cout << RED << "\tFür den appTask " << s << " ist keine 1:1-Abbildung vorhanden! Taskmapper ausführen!\n" << RESET;
        }

        if (one2many) {
            std::cout << "\tFür den appTask " << s << " ist eine 1:N-Abbildung vorhanden.\n" << RESET;
        } else {
            std::cout << RED << "\tFür den appTask " << s << " ist keine 1:N-Abbildung vorhanden! Taskmapper ausführen!\n" << RESET;
        }
    }

    std::cout << "---------------------------------------------------------------------------------------------------------------" << "\n";
    std::cout << "Überprüfung epEBench " << filename_epebench_ebmodels << "\n";

    for (std::string s: apptypeVektor) {
        std::ifstream infile(filename_epebench_ebmodels);
        std::string currentModel;
        std::string line;
        bool found = false;

        while (std::getline(infile, line)) {
            size_t delimiterPos = line.find('=');
            std::string keyInline = (delimiterPos != 0) ? line.substr(0, delimiterPos) : NULL;
            std::string valueInLine = (delimiterPos != 0) ? line.substr(delimiterPos + 1) : NULL;

            if (keyInline == result_section_model) {
                if (s == valueInLine) {
                    found=true;
                }
            }
        }

        if (found) {
            std::cout << "\tFür den appTask " << s << " ist eine 1:N-Abbildung in " << filename_epebench_ebmodels << " vorhanden.\n";
        } else {
            std::cout << RED << "\tFür den appTask " << s << " ist keine 1:N-Abbildung in " << filename_epebench_ebmodels << " vorhanden! Wurde die Taskmap nach epebench übertragen?\n" << RESET;
        }
    }

    std::cout << "---------------------------------------------------------------------------------------------------------------" << "\n";

    std::cout << "Überprüfung Zugriff auf energy_uj:\n\n";
    readEnergy_UJ();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "\n\nÜberprüfung Zugriff auf cpupower frequency-set\n";
    system("cpupower frequency-set");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "\n\nÜberprüfung edgedetection\n";
    system(script_Sample_Application);





}

