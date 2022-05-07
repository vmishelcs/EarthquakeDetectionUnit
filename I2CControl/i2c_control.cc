#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "i2c_control.h"

namespace earthquake_detection_unit {

const std::string kI2CDevLinuxBus1 = "/dev/i2c-1";

I2CControl::I2CControl() {
    fd = open(kI2CDevLinuxBus1.c_str(), O_RDWR | O_NONBLOCK);
    if (fd == -1) {
        std::cerr << "ERROR: unable to open " << kI2CDevLinuxBus1;
        std::cerr << " in I2CManager constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

I2CControl::~I2CControl() {
    close(fd);
}

void I2CControl::SetSlaveAddress(uint8_t device_address) {
    if (ioctl(fd, I2C_SLAVE, device_address)) {
        std::cerr << "ERROR: ioctl failed in Accelerometer constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

void I2CControl::WriteToRegister(uint8_t register_address, uint8_t value) {
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
}

void I2CControl::ReadFromRegister(uint8_t register_address, char *buf, ssize_t buf_size) {
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
}

} // earthquake_detection_unit
