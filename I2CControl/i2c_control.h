#pragma once

#include <cstdint>
#include <mutex>

namespace earthquake_detection_unit {

class I2CControl {
public:
    I2CControl();
    ~I2CControl();

    void SetSlaveAddress(uint8_t device_address);
    void WriteToRegister(uint8_t register_address, uint8_t value);
    void ReadFromRegister(uint8_t register_address, char *buf, ssize_t buf_size);

private:
    // I2C file file descriptor.
    int fd;
    // Mutex for preventing concurrent writes to I2C file.
    std::mutex mtx;
};

} // earthquake_detection_unit
