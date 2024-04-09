
class Task {
public:
    std::string taskfilename;
    std::string taskname;
    std::vector<std::string> sequenzen;
};

class PrototypTask : public Task {
    std::vector<bool> hit; // Übereinstimmung bei Vergleich

    void resetHit(Task anwTask) {
        hit.clear();
        for (std::string s : anwTask.sequenzen) {
            hit.push_back(false);
        }
    }
};

class AnwTask : public Task {
public:
    std::vector<bool> found;
    Result resultOneToOne;
    std::map<std::string, std::list<int>> indexOfMap;

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

    int lastIndexOfTrue() {
        for (int i = found.size()-1; i>0; i--) {
            if (found[i]) {
                return i;
            }
        }
        return 0;
    }


    std::vector<bool> getTaskHitList(PrototypTask ptt) {
        return resultOneToOne.pptFoundMapWithBool[ptt.taskname];
    }

    void initTaskHitList(PrototypTask ptt){
        std::vector<bool> hit;
        for (int i = 0; i < sequenzen.size(); i++) {
            hit.push_back(false);
        }

        resultOneToOne.pptFoundMapWithBool[ptt.taskname]=hit;
    }

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


    void resetFound() {
        found.clear();
        for (std::string s : sequenzen) {
            found.push_back(false);
        }
    }

    int countFound() {
        int count = 0;
        for (bool b : found) {
            if (b) {
                count++;
            }
        }
        return count;
    }

    float percentFound() {
        return (float) countFound() * 100 / sequenzen.size();
    }

    bool allFound() {
        return sequenzen.size() == countFound();
    }

    /**
     * Zählt alle nicht gefundenen bis Count erreicht wurde.
     *
     * @param count
     * @return
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