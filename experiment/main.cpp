#include <iostream>
#include <thread>
#include <ctime>
#include <chrono>
#include <string.h>

void runSampleApplication() {
    std::cout << "Skript runSampleApplication.sh wird gestartet" << std::endl;
    system("./runSampleApplication.sh");
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
    system("./testRaplRead.sh");
}

void testThreadWithRapl() {
    testrapl();
    std::cout << "Thread wird gestartet\n";
    std::thread t1(runSampleApplication);
    std::thread t2(testrapl);
    t1.join();
    t2.join();
    logTime();
}



long long readCounterFromFile() {
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[bufferLength];

    filePointer = fopen("counter.txt", "r");

    //printf("/sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj:\n");

    long long counter = -1;

    while(fgets(buffer, bufferLength, filePointer)) {
        counter = strtol(buffer, NULL, 10);
        //printf("%s\n", buffer);
    }

    fclose(filePointer);



    return counter;
}

long long  readEnergy_UJ() {
    system("./readEnergy_UJ.sh");
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

    //printf("Jahr: %d\n", year);
    //printf("Monat: %d\n", month);
    //printf("Tag: %d\n", local_tm.tm_mday);
    //printf("Std: %d\n", local_tm.tm_hour);
    //printf("Min: %d\n", local_tm.tm_min);

    char* filename;
    sprintf(filename, "%d%s%d%s%d%s%d%s%d.log",
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

void logMeasure(char app[], long long  dauer, long long power) {
    FILE* filePointer;
    filePointer = fopen(getFilename(), "w");
    fprintf(filePointer, "app;duration;power\n");
    fprintf(filePointer, "%s;%lld;%lld\n", app, dauer, power);
    fclose(filePointer);
}


void measureSampleApplication() {
    long long idle3000MS = measureIdle(3000);
    std::cout << "Leistungsaufnahme für 3000MS:" << idle3000MS << std::endl;


    long long counter_beginn = readEnergy_UJ();
    logTime();
    uint64_t begin = timeSinceEpochMillisec();

    std::cout << "Thread wird gestartet\n";
    std::thread t1(runSampleApplication);
    t1.join();

    uint64_t end = timeSinceEpochMillisec();
    uint64_t dauer = end - begin;

    std::cout << "Dauer: " << dauer << " MS" << std::endl;
    logTime();

    long long counter_end = readEnergy_UJ();
    long long counter_diff = counter_end - counter_beginn;
    long long leistungsaufnahme = counter_diff - idle3000MS;

    std::cout << "Leistungsaufnahme in Mikojoul: " << (leistungsaufnahme) << std::endl;
    logMeasure("epeBench", dauer, leistungsaufnahme);
}



int main() {
    //testrapl();
    //testThreadWithRapl();
    measureSampleApplication();



    return 0;
}
