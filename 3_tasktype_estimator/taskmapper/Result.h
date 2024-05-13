#include <iostream>
#include <map>
#include <list>
#include <vector>

#ifndef INC_3_TASKTYPE_ESTIMATOR_RESULT_H
#define INC_3_TASKTYPE_ESTIMATOR_RESULT_H
class Result {
public:
    std::map<std::string, std::map<std::string, int>> protTaskAnzTrefferMap; // <AnwTask, <Prottask, Treffer>>
    std::map<std::string, std::vector<bool>> pptFoundMapWithBool; // für jeden PPT die Treffer speichern
    std::list<std::string> list; // Namen der abgebildeten PPTs.
    std::map<std::string, int> abgebildeteTaskMap;

    void resetPptFoundMap(std::vector<bool> hit) {
        pptFoundMapWithBool.clear();
        /*std::map<std::string, std::vector<bool>>::iterator it;
        for (it = pptFoundMapWithBool.begin(); it != pptFoundMapWithBool.end(); it++) {
            std::string s = it->first;
            pptFoundMapWithBool[s]=hit;
        }*/
    }

};

#endif //INC_3_TASKTYPE_ESTIMATOR_RESULT_H
