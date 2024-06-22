

#ifndef INC_3_TASKTYPE_ESTIMATOR_RESULT_H
#define INC_3_TASKTYPE_ESTIMATOR_RESULT_H
#include <iostream>
#include <map>
#include <list>
#include <vector>

/**
 * Class to hold the results of the task estimation
 */
class Result {
public:
    // Map to hold the number of hits for each prototype task for each application task
    std::map<std::string, std::map<std::string, int>> protTaskAnzTrefferMap; // <AnwTask, <Prottask, Treffer>>

    // Map to store the hits for each prototype task
    std::map<std::string, std::vector<bool>> pptFoundMapWithBool; // for each PPT store the hits

    // List to store the names of the mapped prototype tasks
    std::list<std::string> list; // Names of the mapped PPTs.

    // Map to store the mapped tasks
    std::map<std::string, int> abgebildeteTaskMap;

    /**
     * Function to reset the map that stores the hits for each prototype task
     * @param hit - vector of boolean values representing the hits
     */
    void resetPptFoundMap(std::vector<bool> hit) {
        pptFoundMapWithBool.clear();
    }

};

#endif //INC_3_TASKTYPE_ESTIMATOR_RESULT_H
