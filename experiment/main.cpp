#include <iostream>
#include <thread>
#include <ctime>
#include <chrono>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "measure.cpp"
#include "config.cpp"
#include <ctype.h>
const char* script_Sample_Application = "./scripts/runSampleApplication.sh";
const char* script_testRapl = "./scripts/testRaplRead.sh";


FILE* logfile;

void runSampleApplication(const char* command) {
    std::cout << "Skript " << command << " wird gestartet" << std::endl;
    system(command);
}

// https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
uint64_t timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void logTime() {
    std::time_t result = std::time(nullptr);
    std::cout << std::ctime(&result);
}



void testrapl() {
    system(script_testRapl);
}

void testThreadWithRapl() {
    testrapl();
    std::cout << "Thread wird gestartet\n";

    std::thread t1(runSampleApplication, script_Sample_Application);
    std::thread t2(testrapl);
    t1.join();
    t2.join();
    logTime();
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

long long  readEnergy_UJ() {
    system("./scripts/readEnergy_UJ.sh");
    return readCounterFromFile();
}

long long measureIdle(int milliseconds) {
    long long counter_beginn = readEnergy_UJ();
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    long long counter_end = readEnergy_UJ();
    long long counter_diff = counter_end - counter_beginn;
    return counter_diff;
}

char* getFilename() {
    time_t mytime = time(NULL);
    tm local_tm = *localtime(&mytime);

    int year = local_tm.tm_year + 1900;
    int month = local_tm.tm_mon+1;

    char* filename = (char*) malloc(sizeof(char) * 15);
    sprintf(filename, "%s%d%s%d%s%d%s%d%s%d.log",
            "logs/",
            year,
            (month<10) ? "0" : "",
            month,
            (local_tm.tm_mday<10) ? "0" : "",
            local_tm.tm_mday,
            (local_tm.tm_hour<10) ? "0" : "",
            local_tm.tm_hour,
            (local_tm.tm_min<10) ? "0" : "",
            local_tm.tm_min);

    return filename;
}

void openMeasurFile() {
    mkdir("logs", 0777);
    logfile = fopen(getFilename(), "w");
    fprintf(logfile, "app;duration;power\n");


}

void closeMeasureFile() {
    fclose(logfile);
}

void logMeasure(const char app[], long long  dauer, long long power) {
    fprintf(logfile, "%s;%lld;%lld\n", app, dauer, power);
}


void measureSampleApplication(const char* script) {
    long long idle3000MS = measureIdle(1000);
    std::cout << "Leistungsaufnahme für 3000MS:" << idle3000MS << std::endl;


    long long counter_beginn = readEnergy_UJ();
    logTime();
    uint64_t begin = timeSinceEpochMillisec();

    std::cout << "Thread wird gestartet\n";
    std::thread t1(runSampleApplication, script);
    t1.join();

    uint64_t end = timeSinceEpochMillisec();
    uint64_t dauer = end - begin;

    std::cout << "Dauer: " << dauer << " MS" << std::endl;
    logTime();

    long long counter_end = readEnergy_UJ();
    long long counter_diff = counter_end - counter_beginn;
    long long leistungsaufnahme = counter_diff - idle3000MS;

    std::cout << "Leistungsaufnahme in Mikojoul: " << (leistungsaufnahme) << std::endl;
    logMeasure(script, dauer, leistungsaufnahme);
}

void runSomething() {
    system("./scripts/runDadd.sh");
}





void runAllGenScripts() {
    DIR *dir;
    struct dirent *dent;
    dir = opendir("gen");
    if(dir!=NULL) {
        openMeasurFile();
        while((dent=readdir(dir))!=NULL) {
            char* file = dent->d_name;
            if (strcmp(file, ".") == 0 || strcmp(file, "..") == 0) {
                continue;
            }
            char* filename = (char*) malloc(sizeof(char) * (strlen(file)) + 4);
            sprintf(filename, "%s%s", "gen/", file);

            measureSampleApplication(filename);
        }
        closeMeasureFile();
    }
    printf("%s", "Skripte wurden ausgeführt, Ergebnisse siehe logs-Ordner!");
}

int main(int argc, char *argv[]) {
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
