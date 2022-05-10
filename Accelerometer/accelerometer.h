#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

#include <I2CControl/i2c_control.h>

/*
 * Interface for the MMA8452Q accelerometer.
 * The implementation was written with the help of the following data sheet.
 * https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/MMA8452Q-rev8.1.pdf
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

    Accelerometer();
    ~Accelerometer();

    void ActivateAccelerometer();
    void ShutDownAccelerometer();

    inline double GetCurrentReading() { return current_reading; }
    inline double GetHighestReading() { return highest_reading; }

private:
    typedef struct Vector {
        Vector(int16_t x_reading, int16_t y_reading, int16_t z_reading);
        Vector(const Vector &v);

        double x;
        double y;
        double z;
        double magnitude;
    } Vector;

    // Worker thread for sampling accelerometer readings.
    void Worker();

    // Collects current accelerometer reading via I2C.
    void CollectReading();

    // Sets accelerometer sensitivity via I2C.
    void SetSensitivity(Sensitivity sensitivity);

    // Controller for I2C I/O.
    I2CControl *i2c_c;
    // Accelerometer sensitivity.
    Sensitivity sens;
    // Signal to shut down worker thread.
    std::atomic<bool> shutdown;
    // Worker thread.
    std::thread worker_thread;
    // Exponentially smoothed accelerometer magnitude reading.
    std::atomic<double> current_reading;
    // Highest reading detected by accelerometer.
    std::atomic<double> highest_reading;
};

} // earthquake_detection_unit
