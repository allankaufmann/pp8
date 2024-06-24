

#ifndef INC_3_TASKTYPE_ESTIMATOR_MEASURERESULT_H
#define INC_3_TASKTYPE_ESTIMATOR_MEASURERESULT_H
#include <string>

/**
 * Class to hold the result of a measurement
 */
class MeasureResult {
public:

    // Name of the task
    std::string taskname;

    // Duration of the task
    long long  duration=-1;

    // Energy used by the task in microjoules
    long long energy_mj;

    // CPU frequency during the task
    std::string cpuFreq;

    // Level of parallelism during the task
    std::string parallelism;

    // Duration of the task in a one-to-many scenario
    long long duration_one_to_many;

    // Energy used by the task in a one-to-many scenario in microjoules
    long long energy_my_one_to_many;

    /**
     * Function to calculate the power used by the task
     * @return float - power used by the task, -1 if power is 0
     */
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

    /**
     * Function to calculate the power used by the task in a one-to-many scenario
     * @return float - power used by the task in a one-to-many scenario, -1 if power is 0
     */
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