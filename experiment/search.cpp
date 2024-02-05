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
        std::vector<bool> found;
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
        //printf("%s\n", task);
        t.sequenzen.push_back(task);
    }

    fclose(filePointer);
    return t;
}

void test() {
    std::vector<char*> taskseqnames = readFilenamesFromDirectory(foldername_seq);

    std::vector<PrototypTask> prottaskVektor;
    std::vector<PrototypTask> apptaskVektor;

    for (char* seqname : taskseqnames) {
        PrototypTask t = readSeqFile(seqname);
        prottaskVektor.push_back(t);
        printf("Der Task %s enthält %d Einträge\n", seqname, t.sequenzen.size());
    }
    printf("%s", "\n");
    std::vector<char*> appseqnames = readFilenamesFromDirectory(foldername_appseq);
    for (char* seqname : appseqnames) {
        PrototypTask t = readSeqFile(seqname);
        apptaskVektor.push_back(t);
        printf("Der Task %s enthält %d Einträge\n", seqname, t.sequenzen.size());
    }
    printf("%s", "\n");
    //printf("%d", apptaskVektor.size());

    //std::vector<PrototypTask>::iterator it = apptaskVektor.begin();
    for (PrototypTask t : apptaskVektor) {
        std::cout << "\nAppTask " << t.name << " wird geprüft!\n";

        for (PrototypTask pt : prottaskVektor) {

            for (std::string pts : pt.sequenzen) {
                for (std::string ts : t.sequenzen) {

                    if (pts.compare(ts)==0) {
                        pt.found.push_back(true);
                        break;
                    }
                }
            }
            std::cout << "In AppTask " << t.name << " sind aus ProtTask " << pt.name << " " << pt.found.size() << " Einträge vorhanden!\n";
            pt.found.clear();
        }

        //printf("AppTask %s wird geprüft!", t.name);

    }



    //PrototypTask apptask1 = apptaskVektor.



    //readSeqFile(daddseq);
    //readSeqFile(sampleappseq);

}