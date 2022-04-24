#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "accelerometer.h"

namespace earthquake_detection_unit {

#define I2C_DEV_LINUX_BUS1         "/dev/i2c-1"
#define ACCELEROMETER_I2C_ADDRESS  0x1C
#define ACCELEROMETER_CTRL_REG1    0x2A
#define ACCELEROMETER_XYZ_DATA_CFG 0x0E
#define ACCELEROMETER_ON_CMD       0x01
#define ACCELEROMETER_OFF_CMD      0x00


Accelerometer::Accelerometer() {
    i2c_fd = open(I2C_DEV_LINUX_BUS1, O_RDWR);
    if (i2c_fd == -1) {
        std::cerr << "ERROR: unable to open " << I2C_DEV_LINUX_BUS1;
        std::cerr << " in Accelerometer constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    if (ioctl(i2c_fd, I2C_SLAVE, ACCELEROMETER_I2C_ADDRESS)) {
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
}

void Accelerometer::Worker() {
    while (!shutdown) {

    }
}

void Accelerometer::ReadAccelerometerData(int16_t *data) {
    uint8_t register_address = 0x00;
    // To read from an address, we first write the address.
    if (write(i2c_fd, &register_address, sizeof(register_address)) != sizeof(register_address)) {
        std::cerr << "ERROR: failed to write " << sizeof(register_address);
        std::cerr << " bytes in Accelerometer::ReadAccelerometerData." << std::endl;
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

    }
}

void Accelerometer::ActivateAccelerometer() {
    unsigned char buf[2] = { ACCELEROMETER_I2C_ADDRESS, ACCELEROMETER_ON_CMD };
    if (write(i2c_fd, buf, 2) != 2) {
        std::cerr << "ERROR: failed to write ";
        std::cerr << std::hex << ACCELEROMETER_ON_CMD;
        std::cerr << " to ";
        std::cerr << std::hex << ACCELEROMETER_I2C_ADDRESS;
        std::cerr << " in Accelerometer::ActivateAccelerometer." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

void Accelerometer::ShutdownAccelerometer() {
    unsigned char buf[2] = { ACCELEROMETER_I2C_ADDRESS, ACCELEROMETER_OFF_CMD };
    if (write(i2c_fd, buf, 2) != 2) {
        std::cerr << "ERROR: failed to write ";
        std::cerr << std::hex << ACCELEROMETER_OFF_CMD;
        std::cerr << " to ";
        std::cerr << std::hex << ACCELEROMETER_I2C_ADDRESS;
        std::cerr << " in Accelerometer::ShutdownAccelerometer." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

void Accelerometer::SetSensitivity(Accelerometer::Sensitivity sensitivity) {
    unsigned char buf[2] = { ACCELEROMETER_XYZ_DATA_CFG, sensitivity };
    if (write(i2c_fd, buf, 2) != 2) {
        std::cerr << "ERROR: failed to write ";
        std::cerr << std::hex << sensitivity;
        std::cerr << " to ";
        std::cerr << std::hex << ACCELEROMETER_XYZ_DATA_CFG;
        std::cerr << " in Accelerometer::SetSensitivity." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

} // earthquake_detection_unit

