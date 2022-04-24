#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include <atomic>
#include <cstdint>
#include <thread>

namespace earthquake_detection_unit {

class Accelerometer {
public:
    Accelerometer();
    ~Accelerometer();

private:
    enum Sensitivity {
        SENS_2G, /* 0 */
        SENS_4G, /* 1 */
        SENS_8G  /* 2 */
    };

    // Worker thread for sampling accelerometer readings.
    void Worker();

    void ReadAccelerometerData(int16_t *data);

    void ActivateAccelerometer();
    void ShutdownAccelerometer();

    void SetSensitivity(Sensitivity sensitivity);

    // I2C file descriptor.
    int i2c_fd;
    // Signal to shutdown worker thread.
    std::atomic<bool> shutdown;
    // Worker thread.
    std::thread worker_thread;
};

} // earthquake_detection_unit

#endif // __ACCELEROMETER_H__
