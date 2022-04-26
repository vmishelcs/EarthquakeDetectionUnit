#pragma once

#include <cstdint>
#include <mutex>

namespace earthquake_detection_unit {

class I2CManager {
private:
    static I2CManager *instance;

public:
    inline static void Initialize() { instance = new I2CManager(); }
    inline static void Uninitialize () { delete instance; }
    inline static I2CManager *Get() { return instance; }

    I2CManager(I2CManager const &) = delete;
    void operator=(I2CManager const &) = delete;

    void SetSlaveAddress(uint8_t device_address);
    void WriteToRegister(uint8_t register_address, uint8_t value);
    void ReadFromRegister(uint8_t register_address, char *buf, ssize_t buf_size);

private:
    I2CManager();
    ~I2CManager();

    // I2C file file descriptor.
    int fd;
    // Mutex for preventing concurrent writes to I2C file.
    std::mutex mtx;
};

} // earthquake_detection_unit
