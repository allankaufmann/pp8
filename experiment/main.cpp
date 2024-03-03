#include <iostream>
#include <thread>
#include <string.h>
#include "measure.cpp"
#include "config.cpp"
#include "search.cpp"

const char* script_Sample_Application = "./scripts/runSampleApplication.sh";
const char* script_Sample_vmov = "./scripts/runvmov_SIMD.sh";
const char* script_sobolv = "./scripts/runsobelv.sh";
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

    while ( strcasecmp(parameter, "C")!=0 &&
            strcasecmp(parameter, "A")!=0 &&
            strcasecmp(parameter, "B")!=0 &&
            strcasecmp(parameter, "D")!=0 &&
            strcasecmp(parameter, "E")!=0 &&
            strcasecmp(parameter, "R")!=0 &&
            strcasecmp(parameter, "S")!=0 &&
            strcasecmp(parameter, "T")!=0 &&
            strcasecmp(parameter, "X")!=0 &&
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
        printf("\tT (=Test: 10xLeerlauf messen)\n");
        printf("\tV (=Versuch...epebench-sobelv messen)\n");
        printf("\tX (=Exit)\n");
        char inputParameter[1];
        scanf("%s", inputParameter);
        parameter=inputParameter;
    }

    if (strcasecmp(parameter, "X")==0) {
        return 0;
    } else if (strcasecmp(parameter, "C") == 0) {
        readConfigFile(); // Konfigurationsdatei auslesen und Skripte erstellen
    } else if (strcasecmp(parameter, "R")==0) {
        runAllGenScripts(3, foldername_generated_scripts); // Messungen der Tasks
    } else if (strcasecmp(parameter, "S")==0) {
        runAllGenScripts(10, foldername_edgedetction_scripts);
        printf("%s", "Beispielanwendung wurde gemessen, Ergebniss siehe logs-Ordner!");
    } else if (strcasecmp(parameter, "A")==0) {
        compareAppTaskProtTasksOneToOne(true);
    } else if (strcasecmp(parameter, "B")==0) {
        compareAppTaskProtTasksOneToOne(false);
    }   else if (strcasecmp(parameter, "D")==0) {
        compareAppTaskProtTasksOneToMany(true);
    } else if (strcasecmp(parameter, "E")==0) {
        compareAppTaskProtTasksOneToMany(false);
    }  else if (strcasecmp(parameter, "T")==0) {


        for (int i = 0; i < 10 ; i++) {
            printf("\n%d %lu", i, measureIdle(1000));
            //measureIdle(1000);
            //printf("\nCounter: %lld\n",readEnergy_UJ());
        }

    } else if (strcasecmp(parameter, "V")==0) {
        //compareAppTaskProtTasksOneToMany(true);
        openMeasurFile();
        measureSampleApplication(script_sobolv);
        //measureSampleApplication(script_Sample_m4x4smul_SIMD);
        closeMeasureFile();
        //testrapl();
        //testThreadWithRapl();
        //measureSampleApplication(script_Sample_Application);
        //measureSampleApplication("./scripts/runDadd.sh");
        //measureSampleApplication("./scripts/runm4x4smul_SIMD.sh");
    }
    return 0;
}
