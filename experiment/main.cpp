#include <iostream>
#include <thread>
#include <string.h>
#include "measure.cpp"
#include "config.cpp"
#include "search.cpp"

const char* script_Sample_Application = "./scripts/runCommand.sh";
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
        //test();
        //return 0;

    char *parameter;

    if (argc==2) {
        parameter= argv[1];
    }

    while (strcasecmp(parameter, "C")!=0 &&
            strcasecmp(parameter, "R")!=0 &&
            strcasecmp(parameter, "S")!=0 &&
            strcasecmp(parameter, "X")!=0
            )  {
        printf("Bitte einen der folgenden Parameter eingeben: C (=Config), R (=Run), S (=SampleApp), X (=Exit)");
        char inputParameter[1];
        scanf("%s", inputParameter);
        parameter=inputParameter;
    }

    if (strcasecmp(parameter, "X")==0) {
        return 0;
    } else if (strcasecmp(parameter, "C") == 0) {
        readConfigFile(); // Konfigurationsdatei auslesen und Skripte erstellen
    } else if (strcasecmp(parameter, "R")==0) {
        runAllGenScripts(); // Messungen der Tasks
    } else if (strcasecmp(parameter, "S")==0) {
        measureSampleApplication(script_Sample_Application); // Beispielanwendung
        printf("%s", "Beispielanwendung wurde gemessen, Ergebniss siehe logs-Ordner!");
    }


    //testrapl();
    //testThreadWithRapl();
    //measureSampleApplication(script_Sample_Application);
    //measureSampleApplication("./scripts/runDadd.sh");
    //measureSampleApplication("./scripts/runm4x4smul_SIMD.sh");


    return 0;
}
