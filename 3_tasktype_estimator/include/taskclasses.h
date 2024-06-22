

#ifndef INC_3_TASKTYPE_ESTIMATOR_TASKCLASSES_H
#define INC_3_TASKTYPE_ESTIMATOR_TASKCLASSES_H
#include <string>
#include <vector>
#include <map>
#include "Result.h"
/**
 * Base class for tasks
 */
class Task {
public:
    std::string taskfilename; // Filename of the task
    std::string taskname; // Name of the task
    std::vector<std::string> sequenzen; // Sequences of the task
};

/**
 * Class for prototype tasks, inherits from Task
 */
class PrototypTask : public Task {
public:
    std::vector<bool> hit; // Übereinstimmung bei Vergleich
    std::map<std::string, std::map<std::string, bool>> uniqueEntryPairMap; // für Pairsearch!

    /**
     * Initialize hit vector
     */
    void initHit() {
        hit.clear();
        for (std::string s : sequenzen) {
            hit.push_back(false);
        }
    }

    /**
     * Count the number of hits
     * @return int - number of hits
     */
    int countNotFound() {
        int count = 0;
        for (bool b : hit) {
            if (!b) {
                count++;
            }
        }
        return count;
    }

};

/**
 * Class for application tasks, inherits from Task
 */
class AnwTask : public Task {
public:
    std::vector<bool> found; // Übereinstimmung bei Vergleich
    Result resultOneToOne; // Result of the one-to-one comparison
    std::map<std::string, std::list<int>> indexOfMap; // Map to store the index of sequences

    /**
     * Initialize the index of the map
     * @param maxIndex - maximum index
     * @param prottaskname - name of the prototype task
     */
    void initIndexOfMap(int maxIndex, std::string prottaskname) {
        indexOfMap.clear();
        for (int i = 0; i < sequenzen.size(); i++) {
            if (maxIndex!=0 && i>maxIndex) {
                return;
            }
            if (found[i]){
                continue;
            }
            if (resultOneToOne.pptFoundMapWithBool[prottaskname][i]) {
                continue;
            }
            indexOfMap[sequenzen[i]].push_back(i);
        }
    }

    /**
     * Get the last index of true
     * @return int - last index of true
     */
    int lastIndexOfTrue() {
        for (int i = found.size()-1; i>0; i--) {
            if (found[i]) {
                return i;
            }
        }
        return 0;
    }

    /**
     * Get the hit list for a task
     * @param ptt - prototype task
     * @return vector of bool - hit list
     */
    std::vector<bool> getTaskHitList(PrototypTask ptt) {
        return resultOneToOne.pptFoundMapWithBool[ptt.taskname];
    }

    /**
     * Initialize the hit list for a task
     * @param ptt - prototype task
     */
    void initTaskHitList(PrototypTask ptt){
        std::vector<bool> hit;
        for (int i = 0; i < sequenzen.size(); i++) {
            hit.push_back(false);
        }

        resultOneToOne.pptFoundMapWithBool[ptt.taskname]=hit;
    }

    /**
     * Merge the found vector with a given hit vector
     * @param hit - Hit vector to merge with the found vector
     */
    void mergeFound(std::vector<bool> hit) {
        for (int i = 0; i < sequenzen.size(); i++) {
            if (found[i]) {
                continue;
            }
            if (hit[i]) {
                found[i]=true;
            }
        }
    }

    /**
    * Reset the found vector
    */
    void resetFound() {
        found.clear();
        for (std::string s : sequenzen) {
            found.push_back(false);
        }
    }

    /**
    * Count the number of found sequences
    * @return int - Number of found sequences
    */
    int countFound() {
        int count = 0;
        for (bool b : found) {
            if (b) {
                count++;
            }
        }
        return count;
    }

    /**
    * Get the percentage of found sequences
    * @return float - Percentage of found sequences
    */
    float percentFound() {
        return (float) countFound() * 100 / sequenzen.size();
    }

    /**
    * Check if all sequences were found
    * @return bool - True if all sequences were found, false otherwise
    */
    bool allFound() {
        return sequenzen.size() == countFound();
    }

    /**
     * Zählt alle nicht gefundenen bis Count erreicht wurde.
     *
     * @param count
     * maxIndex  - maximaler Index
     */
    int maxIndex(int max) {
        int count = 0;
        for (int i = 0; i < found.size(); i++) {
            if (!found[i]) {
                count++;
            }
            if (count>=max){
                return i;
            }
        }
        return found.size()-1;
    }

    std::string bestName;
    float besthit = 0;
    int bestFirstFalseIndex = 0;

    void calcBestTask() {
        std::map<std::string, int>::iterator it;
        for (it = resultOneToOne.protTaskAnzTrefferMap[taskname].begin(); it != resultOneToOne.protTaskAnzTrefferMap[taskname].end(); it++) {
            if (it->second>besthit) {
                besthit=it->second;
                bestName=it->first;
            }
        }
    }


};
#endif //INC_3_TASKTYPE_ESTIMATOR_TASKCLASSES_H
