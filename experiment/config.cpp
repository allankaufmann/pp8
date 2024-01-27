#include <iostream>
#include <string.h> //strtok
#include <sys/stat.h> //mkdir

const char* config_filename = "experiment.ini";

void writeGenScript(char* task) {
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
    mkdir("gen", 0777);
    FILE* filePointer;
    filePointer = fopen(config_filename, "r");
    int bufferLength = 255;
    char buffer[bufferLength];

    char* task;

    while(fgets(buffer, bufferLength, filePointer)) {
        task = strtok(buffer, "=");
        writeGenScript(task);
    }

    fclose(filePointer);
    printf("%s", "Skripte zum Ausführen der prototypischen Tasks wurden im Ordner 'gen' erstellt!");
}

