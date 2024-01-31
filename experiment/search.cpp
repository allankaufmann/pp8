#include <iostream>
#include <string.h> //strtok
#include <vector>
#include "tools.hpp"
#include "constants.h"

const char* daddseq= "search/taskseq/dadd.seq";
const char* sampleappseq = "search/appseq/sampleapp.seq";

class PrototypTask {
    public:
        std::string name;
        std::vector<std::string> sequenzen;
};

PrototypTask readSeqFile(const char* file) {
    PrototypTask t;
    t.name=file;


    FILE* filePointer;
    filePointer = fopen(file, "r");

    int bufferLength = 255;
    char buffer[bufferLength];

    char* task;

    while(fgets(buffer, bufferLength, filePointer)) {
        task = strtok(buffer, " ");
        printf("%s\n", task);
        //t.sequenzen.push_back(task);
    }

    fclose(filePointer);
    return t;
}

void test() {
    std::vector<char*> seqnames = readFilenamesFromDirectory(foldername_seq);
    for (char* seqname : seqnames) {
        PrototypTask t = readSeqFile(seqname);
        //printf("Der Task %s enthält %d Einträge\n", seqname, t.sequenzen.size());
    }
    //readSeqFile(daddseq);
    //readSeqFile(sampleappseq);

}