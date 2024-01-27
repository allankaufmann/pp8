#include <iostream>
#include <string.h> //strtok
const char* daddseq= "search/taskseq/dadd.seq";
const char* sampleappseq = "search/appseq/sampleapp.seq";

void readSeqFile(const char* file) {
    FILE* filePointer;
    filePointer = fopen(daddseq, "r");

    int bufferLength = 255;
    char buffer[bufferLength];

    char* task;

    while(fgets(buffer, bufferLength, filePointer)) {
        task = strtok(buffer, " ");
        printf("%s\n", task);
    }

    fclose(filePointer);

}

void test() {
    readSeqFile(daddseq);
    readSeqFile(sampleappseq);

}