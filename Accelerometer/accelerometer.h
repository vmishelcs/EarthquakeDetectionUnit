#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include <atomic>

namespace earthquake_detection_unit {

class Accelerometer {
public:
    Accelerometer();
    ~Accelerometer();

private:
    // I2C file descriptor.
    int i2c_fd;
    // Signal to shutdown worker thread.
    std::atomic<bool> shutdown;
};

} // earthquake_detection_unit

#endif // __ACCELEROMETER_H__
