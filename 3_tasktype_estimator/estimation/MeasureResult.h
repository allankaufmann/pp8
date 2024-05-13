

#include <string>
#ifndef INC_3_TASKTYPE_ESTIMATOR_MEASURERESULT_H
#define INC_3_TASKTYPE_ESTIMATOR_MEASURERESULT_H

class MeasureResult {
public:

    std::string taskname;
    long long  duration;
    long long energy_mj;
    std::string cpuFreq;
    std::string parallelism;

    long long duration_one_to_many;
    long long energy_my_one_to_many;

    float power() {
        if (energy_mj == 0 || duration==0) {
            return 0;
        }
        float power = (float) energy_mj/duration;
        if (power==0) {
            return -1;
        }
        return power;
    }

    float powerOneToMany() {
        if (energy_mj == 0 || duration==0) {
            return 0;
        }
        float power = (float) energy_my_one_to_many/duration_one_to_many;
        if (power==0) {
            return -1;
        }
        return power;
    }
};
#endif //INC_3_TASKTYPE_ESTIMATOR_MEASURERESULT_H
