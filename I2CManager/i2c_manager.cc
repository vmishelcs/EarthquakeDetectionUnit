#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "i2c_manager.h"

namespace earthquake_detection_unit {

const std::string kI2CDevLinuxBus1 = "/dev/i2c-1";

I2CManager *I2CManager::instance = nullptr;

I2CManager::I2CManager() {
    fd = open(kI2CDevLinuxBus1.c_str(), O_RDWR);
    if (fd == -1) {
        std::cerr << "ERROR: unable to open " << kI2CDevLinuxBus1;
        std::cerr << " in I2CManager constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

I2CManager::~I2CManager() {
    close(fd);
}

void I2CManager::SetSlaveAddress(uint8_t device_address) {
    mtx.lock();

    if (ioctl(fd, I2C_SLAVE, device_address)) {
        std::cerr << "ERROR: ioctl failed in Accelerometer constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    mtx.unlock();
}

void I2CManager::WriteToRegister(uint8_t register_address, uint8_t value) {
    mtx.lock();

    uint8_t buf[2] = { register_address, value };
    if (write(fd, buf, 2) != 2) {
        std::cerr << "ERROR: failed to write ";
        printf("%#04x", buf[1]);
        std::cerr << " to ";
        printf("%#04x", buf[0]);
        std::cerr << " in I2CManager::WriteToRegister." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    mtx.unlock();
}

void I2CManager::ReadFromRegister(uint8_t register_address, char *buf, ssize_t buf_size) {
    mtx.lock();

    // To read from an address, we first write the address.
    if (write(fd, &register_address, 1) != 1) {
        std::cerr << "ERROR: failed to write register address";
        std::cerr << " in I2CManager::ReadFromRegister." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    // Read from specified register into buffer.
    if (read(fd, buf, buf_size) != buf_size) {
        std::cerr << "ERROR: failed to read " << buf_size;
        std::cerr << " bytes in I2CManager::ReadFromRegister." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    mtx.unlock();
}

} // earthquake_detection_unit
