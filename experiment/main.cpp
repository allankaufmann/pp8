#include <iostream>
#include <thread>
#include <ctime>
#include <chrono>

void doSomething() {
    system("./test.sh");
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

void testepebench() {
    logTime();
    uint64_t begin = timeSinceEpochMillisec();

    std::cout << "Thread wird gestartet\n";
    std::thread t1(doSomething);
    t1.join();

    uint64_t end = timeSinceEpochMillisec();
    uint64_t dauer = end - begin;

    std::cout << "Dauer: " << dauer << " MS" << std::endl;
    logTime();
}

void testrapl() {
    system("./testRaplRead.sh");
}

void testThreadWithRapl() {
    testrapl();
    std::cout << "Thread wird gestartet\n";
    std::thread t1(doSomething);
    std::thread t2(testrapl);
    t1.join();
    t2.join();
    logTime();
}

void testpowercap() {
    system("./testpowercap.sh");
}

int main() {
    //testepebench();
    //testrapl();
    //testThreadWithRapl();
    testpowercap();
    return 0;
}
