#include <cerrno>
#include <cmath>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "accelerometer.h"

namespace earthquake_detection_unit {

const std::string kI2CDevLinuxBus1     = "/dev/i2c-1";
const uint8_t kAccelerometerI2CAddress = 0x1C;
const uint8_t kAccelerometerCtrlReg1   = 0x2A;
const uint8_t kAccelerometerXYZDataCFG = 0x0E;
const uint8_t kAccelerometerOnCommand  = 0x01;
const uint8_t kAccelerometerOffCommand = 0x00;
const uint8_t kSleepTime_ms            = 2;
const uint8_t kSampleBufferSize        = 16;


Accelerometer::Accelerometer() : shutdown(false), samples() {
    i2c_fd = open(kI2CDevLinuxBus1.c_str(), O_RDWR);
    if (i2c_fd == -1) {
        std::cerr << "ERROR: unable to open " << kI2CDevLinuxBus1;
        std::cerr << " in Accelerometer constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    if (ioctl(i2c_fd, I2C_SLAVE, kAccelerometerI2CAddress)) {
        std::cerr << "ERROR: ioctl failed in Accelerometer constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    ActivateAccelerometer();

    SetSensitivity(Accelerometer::Sensitivity::SENS_2G);

    worker_thread = std::thread(&Accelerometer::Worker, this);
}

Accelerometer::~Accelerometer() {
    shutdown = true;
    worker_thread.join();
    close(i2c_fd);
}

Accelerometer::Vector::Vector(int16_t x_reading, int16_t y_reading, int16_t z_reading) {
    // Obtain acceleration magnitudes.
    x = (double)abs(x_reading > 2048 ? x_reading - 4095 : x_reading);
    y = (double)abs(y_reading > 2048 ? y_reading - 4095 : y_reading);
    z = (double)abs(z_reading > 2048 ? z_reading - 4095 : z_reading);
    x /= 1024.0f;
    y /= 1024.0f;
    z /= 1024.0f;
    // Obtain vector magnitude.
    magnitude = sqrt(x * x + y * y + z * z);
}

Accelerometer::Vector::Vector(const Vector &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    magnitude = v.magnitude;
}

void Accelerometer::Worker() {
    while (!shutdown) {
        CollectReading();

        // Gather samples every 2 ms.
        std::this_thread::sleep_for(std::chrono::milliseconds(kSleepTime_ms));
    }
}

void Accelerometer::CollectReading() {
    uint8_t register_address[] = {0x00};
    // To read from an address, we first write the address.
    if (write(i2c_fd, register_address, 1) != 1) {
        std::cerr << "ERROR: failed to write " << 1;
        std::cerr << " byte in Accelerometer::ReadAccelerometerData." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    // Buffer for storing accelerometer reading.
    uint8_t buf[7];
    memset(buf, 0, sizeof(*buf) * 7);

    // Read accelerometer data.
    if (read(i2c_fd, buf, 7) != 7) {
        std::cerr << "ERROR: failed to read accelerometer data";
        std::cerr << " in Accelerometer::ReadAccelerometerData." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
    else {
        int16_t x_reading = ((buf[1] << 8) | buf[2]) >> 4;
        int16_t y_reading = ((buf[3] << 8) | buf[4]) >> 4;
        int16_t z_reading = ((buf[5] << 8) | buf[6]) >> 4;

        mtx.lock();
        {
            // Insert the latest reading at the front.
            samples.emplace(samples.begin(), x_reading, y_reading, z_reading);
            // Do not store more than kSampleBufferSize readings.
            if (samples.size() > kSampleBufferSize) {
                samples.pop_back();
            }
        }
        mtx.unlock();
    }
}

Accelerometer::Vector Accelerometer::GetReading() {
    if (samples.size() > 0) {
        mtx.lock();
        Vector result = samples.front();
        mtx.unlock();
        return result;
    }
    return Vector(0, 0, 0);
}

void Accelerometer::ActivateAccelerometer() {
    unsigned char buf[2] = { kAccelerometerCtrlReg1, kAccelerometerOnCommand };
    if (write(i2c_fd, buf, 2) != 2) {
        std::cerr << "ERROR: failed to write ";
        std::cerr << std::hex << buf[1];
        std::cerr << " to ";
        std::cerr << std::hex << buf[0];
        std::cerr << " in Accelerometer::ActivateAccelerometer." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

void Accelerometer::ShutdownAccelerometer() {
    unsigned char buf[2] = { kAccelerometerCtrlReg1, kAccelerometerOffCommand };
    if (write(i2c_fd, buf, 2) != 2) {
        std::cerr << "ERROR: failed to write ";
        std::cerr << std::hex << buf[1];
        std::cerr << " to ";
        std::cerr << std::hex << buf[0];
        std::cerr << " in Accelerometer::ShutdownAccelerometer." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

void Accelerometer::SetSensitivity(Accelerometer::Sensitivity sensitivity) {
    unsigned char buf[2] = { kAccelerometerXYZDataCFG, sensitivity };
    if (write(i2c_fd, buf, 2) != 2) {
        std::cerr << "ERROR: failed to write ";
        std::cerr << std::hex << buf[1];
        std::cerr << " to ";
        std::cerr << std::hex << buf[0];
        std::cerr << " in Accelerometer::SetSensitivity." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

} // earthquake_detection_unit

