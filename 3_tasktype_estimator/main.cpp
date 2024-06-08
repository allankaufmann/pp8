#include "include/constants.h"
#include "include/config.h"
#include "include/measure.h"
#include "include/taskmapperLog.h"
#include "include/taskmapperOneToOne.h"
#include "include/taskmapperOneToMany.h"
#include "include/estimator.h"
#include "include/taskmapperDiffsearch.h"
#include "include/taskmapperPairsearch.h"



int main(int argc, char *argv[]) {
    char *parameter = nullptr;
    int testAppTaskIndex = -1;

    if (argc==2) {
        parameter= argv[1];
    } else if (argc==3) {
        parameter= argv[1];
        testAppTaskIndex=std::stoi(argv[2]);
    }

    while (
            parameter == nullptr || (
                    strcasecmp(parameter, "C")!=0 &&
                    strcasecmp(parameter, "I")!=0 &&
                    strcasecmp(parameter, "A")!=0 &&
                    strcasecmp(parameter, "B")!=0 &&
                    strcasecmp(parameter, "D")!=0 &&
                    strcasecmp(parameter, "E")!=0 &&
                    strcasecmp(parameter, "R")!=0 &&
                    strcasecmp(parameter, "S")!=0 &&
                    strcasecmp(parameter, "T")!=0 &&
                    strcasecmp(parameter, "U")!=0 &&
                    strcasecmp(parameter, "Z")!=0 &&
                    strcasecmp(parameter, "X")!=0 &&
                    strcasecmp(parameter, "Y")!=0 &&
                    strcasecmp(parameter, "W")!=0 &&
                    strcasecmp(parameter, "Q")!=0 &&
                    strcasecmp(parameter, "V")!=0)
            )  {
        printf("Bitte einen der folgenden Parameter eingeben: \n");

        printf("\t-------------------------- Init -------------------------------------------------------------------\n");
        printf("\tI (=Init - Vorbedingungen für Experiment prüfen!)\n");
        printf("\tC (=Config - Konfigurationsdatei auslesen und Skripte für prototyptasks erstellen)\n");

        printf("\t----------------------- Taskmapper-----------------------------------------------------------------\n");
        printf("\tA (Abbildung - 1 AnwTask auf 1 Prototyptask - Test)\n");
        printf("\tB (Abbildung - 1 AnwTask auf 1 Prototyptask - alle)\n");
        printf("\tD (Abbildung - 1 AnwTask auf N Prototyptasks - Test)\n");
        printf("\tE (Abbildung - 1 AnwTask auf N Prototyptasks - alle)\n");
        printf("\tT (=Transfer to epebench)\n");

        printf("\t---------------------- Tasktype Estimator ---------------------------------------------------------\n");
        printf("\tR (Run - Leistungsaufnahme aller prototyptasks messen)\n");
        printf("\tS (Anw. Tasks messen)\n");
        printf("\tU (Estimation - alle)\n");
        printf("\tZ (Estimation - TEST)\n");

        printf("\t---------------------------- Sonstiges ------------------------------------------------------------\n");
        printf("\tV (Alternative Abbildung - Diffsuche)\n");
        printf("\tY (Alternative Abbildung - Paarsuche)\n");
        printf("\tQ (Alternative Abbildung - Drillingsuche)\n");
        printf("\tW (CPU Frequence)\n");
        printf("\tX (=Exit)\n");
        char inputParameter[1];
        scanf("%s", inputParameter);
        parameter=inputParameter;
    }

    if (strcasecmp(parameter, "I") == 0) {
        checkPreconditions();
    } else if (strcasecmp(parameter, "C") == 0) {
        readConfigFile(true, true); // Konfigurationsdatei auslesen und Skripte erstellen
    } else if (strcasecmp(parameter, "A")==0) {
        initTaskVektors();
        printf("Im Ordner taskmapper/appseq sind %lu Skripte hinterlegt, bitte durch Eingabe auswählen!\n", apptaskVektor.size());

        for (int i = 0; i < apptaskVektor.size(); i++) {
            printf("[%d]: %s\n", i, apptaskVektor[i].taskname.c_str());
        }

        int index = 0;
        if (scanf("%d", &index) == 1) {
            compareAppTaskProtTasksOneToOneTest(apptaskVektor[index].taskname);
        }
    } else if (strcasecmp(parameter, "B")==0) {
        compareAppTaskProtTasksOneToOne();
    }   else if (strcasecmp(parameter, "D")==0) {
        initTaskVektors();
        printf("Im Ordner taskmapper/appseq sind %lu Skripte hinterlegt, bitte durch Eingabe auswählen!\n", apptaskVektor.size());

        for (int i = 0; i < apptaskVektor.size(); i++) {
            printf("[%d]: %s\n", i, apptaskVektor[i].taskname.c_str());
        }

        int index = 0;
        if (scanf("%d", &index) == 1) {
            compareAppTaskProtTasksOneToManyTest(apptaskVektor[index].taskname);
        }
    } else if (strcasecmp(parameter, "E")==0) {
        compareAppTaskProtTasksOneToMany();
    } else if (strcasecmp(parameter, "T")==0) {
        transferTaskMapToEpEBench();
    } else if (strcasecmp(parameter, "R")==0) {
        measureAllPrototypTasks(measure_repeats);
    } else if (strcasecmp(parameter, "S")==0) {
        runAndMeasureScriptsFromDirectory(1, foldername_generated_scripts_apptasks.c_str(), "1");
        printf("%s", "Beispielanwendung wurde gemessen, Ergebniss siehe logs-Ordner!");
    } else if (strcasecmp(parameter, "U")==0) {
        readConfigFile(false, false);
        startEstimation(measure_repeats);
    } else if (strcasecmp(parameter, "Z")==0) {
        readConfigFile(false, false);
        initTaskVektors();

        if (testAppTaskIndex == -1) {
            printf("Folende AppTasks sind konfiguriert, bitte durch Eingabe auswählen!\n");

            for (int i = 0; i < apptaskVektor.size(); i++) {
                printf("[%d]: %s\n", i, apptaskVektor[i].taskname.c_str());
            }
            int index = 0;
            if (scanf("%d", &index) == 1) {
                testEstimation(apptaskVektor[index].taskname, measure_repeats);
            }
        } else {
            testEstimation(apptaskVektor[testAppTaskIndex].taskname, measure_repeats);
        }
    } else if (strcasecmp(parameter, "V")==0) {
        openMeasurLogFile();
        testDiffSearch();
        closeMeasureLogFile();
    } else if (strcasecmp(parameter, "Y")==0) {
        openMeasurLogFile();
        testPairSearch();
        closeMeasureLogFile();
    } else if (strcasecmp(parameter, "Q")==0) {
        openMeasurLogFile();
        testTripleSearch();
        closeMeasureLogFile();
    }  else if (strcasecmp(parameter, "W")==0) {
        selectCpuFrequency();
    }  else if (strcasecmp(parameter, "X")==0) {
        return 0;
    }
    return 0;
}
