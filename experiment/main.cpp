#include <iostream>
#include <thread>
#include <string.h> //strtok

#include "config.cpp"
#include "estimation/measure.cpp"
#include "math.h" //ceil
#include "taskmapper/taskmapperresult.cpp"
#include "taskmapper/taskclasses.cpp"
#include "taskmapper/taskmapperLog.cpp"
#include "taskmapper/taskmapperOneToOne.cpp"
#include "taskmapper/taskmapperOneToMany.cpp"
#include "taskmapper/taskmapperDiffsearch.cpp"
#include "taskmapper/taskmapperPairsearch.cpp"
#include "estimation/estimator.cpp"


const char* script_Sample_Application = "./scripts/runSampleApplication.sh";
const char* script_Sample_vmov = "./scripts/runvmov_SIMD.sh";
const char* script_sobolv = "./scripts/runsobelv.sh";
const char* script_measure = "./scripts/measureGreyscale1N.sh";
const char* script_Sample_m4x4smul_SIMD = "./scripts/runm4x4smul_SIMD.sh";

const char* script_testRapl = "./scripts/testRaplRead.sh";




void testrapl() {
    system(script_testRapl);
}

void testThreadWithRapl() {
    testrapl();
    std::cout << "Thread wird gestartet\n";

    std::thread t1(runCommand, script_Sample_Application);
    std::thread t2(testrapl);
    t1.join();
    t2.join();
    logTime();
}

void runSomething() {
    system("./scripts/runDadd.sh");
}



int main(int argc, char *argv[]) {
    char *parameter;

    if (argc==2) {
        parameter= argv[1];
    }

    while (
            //strcasecmp(parameter, "1")!=0 &&
            strcasecmp(parameter, "C")!=0 &&
            strcasecmp(parameter, "A")!=0 &&
            strcasecmp(parameter, "B")!=0 &&
            strcasecmp(parameter, "D")!=0 &&
            strcasecmp(parameter, "E")!=0 &&
            strcasecmp(parameter, "R")!=0 &&
            strcasecmp(parameter, "S")!=0 &&
            strcasecmp(parameter, "T")!=0 &&
            strcasecmp(parameter, "U")!=0 &&
            strcasecmp(parameter, "X")!=0 &&
            strcasecmp(parameter, "W")!=0 &&
            strcasecmp(parameter, "V")!=0
            )  {
        printf("Bitte einen der folgenden Parameter eingeben: \n");
        printf("\tC (=Config - Konfigurationsdatei auslesen und Skripte für prototyptasks erstellen)\n");
        printf("\tA (=Abbildung - 1 AnwTask auf 1 Prototyptask - Test)\n");
        printf("\tB (=Abbildung - 1 AnwTask auf 1 Prototyptask - alle)\n");
        printf("\tD (=Abbildung - 1 AnwTask auf N Prototyptasks - Test)\n");
        printf("\tE (=Abbildung - 1 AnwTask auf N Prototyptasks - alle)\n");
        printf("\tR (=Run - Leistungsaufnahme aller prototyptasks messen)\n");
        printf("\tS (=Anw. Tasks messen)\n");
        printf("\tT (=Test: Alternatives Mapping)\n");
        printf("\tU (=Estimation)\n");
        printf("\tV (=Versuch...greyscale 1:N)\n");
        printf("\tW (CPU Frequence)\n");
        printf("\tX (=Exit)\n");
        char inputParameter[1];
        scanf("%s", inputParameter);
        parameter=inputParameter;
    }

    if (strcasecmp(parameter, "X")==0) {
        return 0;
    } /*else if (strcasecmp(parameter, "1") == 0) {
        printf("hallo");
    }*/ else if (strcasecmp(parameter, "C") == 0) {
        readConfigFile(true, true); // Konfigurationsdatei auslesen und Skripte erstellen
    } else if (strcasecmp(parameter, "R")==0) {
        measureAllPrototypTasks(3);
        //runAndMeasureScriptsFromDirectory(3, foldername_generated_scripts_tasktypes.c_str(), "1"); // Messungen der Tasks
    } else if (strcasecmp(parameter, "S")==0) {
        runAndMeasureScriptsFromDirectory(1, foldername_generated_scripts_apptasks.c_str(), "1");
        printf("%s", "Beispielanwendung wurde gemessen, Ergebniss siehe logs-Ordner!");
    } else if (strcasecmp(parameter, "A")==0) {
        initTaskVektors();
        printf("Im Ordner taskmapper/appseq sind %d Skripte hinterlegt, bitte durch Eingabe auswählen!\n", apptaskVektor.size());

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
        printf("Im Ordner taskmapper/appseq sind %d Skripte hinterlegt, bitte durch Eingabe auswählen!\n", apptaskVektor.size());

        for (int i = 0; i < apptaskVektor.size(); i++) {
            printf("[%d]: %s\n", i, apptaskVektor[i].taskname.c_str());
        }

        int index = 0;
        if (scanf("%d", &index) == 1) {
            compareAppTaskProtTasksOneToManyTest(apptaskVektor[index].taskname);
        }
    } else if (strcasecmp(parameter, "E")==0) {
        compareAppTaskProtTasksOneToMany();
    } else if (strcasecmp(parameter, "W")==0) {
        selectCpuFrequency();
    }  else if (strcasecmp(parameter, "T")==0) {

        //testDiffSearch();
        testPairSearch();
        //testTripleSearch();
        //selectCpuFrequency();
        /*for (int i = 0; i < 10 ; i++) {
            printf("\n%d %lu", i, measureIdle(1000));
            //measureIdle(1000);
            //printf("\nCounter: %lld\n",readEnergy_UJ());
        }*/



    } else if (strcasecmp(parameter, "V")==0) {

        openMeasurLogFile();
        runAndMeasureScript(script_measure);
        closeMeasureLogFile();

        /*openLogfileTaskmapper();
        saveLineToTaskmapFile("greyscale", " dadd=0.13");
        saveLineToTaskmapFile("greyscale", " x=0.99");
        saveLineToTaskmapFile("neuesModell", " x=0.99");
        saveLineToTaskmapFile("neuesModell", " y=0.01");*/

    } else if (strcasecmp(parameter, "U")==0) {
        readConfigFile(false, false);
        testEstimation();
    }
    return 0;
}
