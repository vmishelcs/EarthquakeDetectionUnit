#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

/*
 * Interface for the MMA8452Q accelerometer.
 * The implementation was written with the help of the following data sheet.
 * https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/MMA8452Q-rev8.1.pdf
 * 
 * 
 */

namespace earthquake_detection_unit {

class Accelerometer {
public:
    enum Sensitivity {
        SENS_2G, /* 0 */
        SENS_4G, /* 1 */
        SENS_8G  /* 2 */
    };

    typedef struct Vector {
        Vector(int16_t x_reading, int16_t y_reading, int16_t z_reading);
        Vector(const Vector &v);

        double x;
        double y;
        double z;
        double magnitude;
    } Vector;

    Accelerometer();
    Accelerometer(Sensitivity sens);
    ~Accelerometer();

    inline double GetCurrentReading() { return current_reading; }
    inline double GetHighestReading() { return highest_reading; }

private:
    // Worker thread for sampling accelerometer readings.
    void Worker();

    void CollectReading();

    void ActivateAccelerometer();
    void ShutdownAccelerometer();

    void SetSensitivity(Sensitivity sensitivity);

    // Accelerometer sensitivity.
    Sensitivity sens;
    // Signal to shutdown worker thread.
    std::atomic<bool> shutdown;
    // Worker thread.
    std::thread worker_thread;
    // Exponentially smoothed accelerometer magnitude reading.
    std::atomic<double> current_reading;
    std::atomic<double> highest_reading;
};

} // earthquake_detection_unit
