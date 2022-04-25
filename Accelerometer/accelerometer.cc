#include <cmath>
#include <cstring>
#include <iostream>

#include "accelerometer.h"
#include <I2CManager/i2c_manager.h>

namespace earthquake_detection_unit {

const uint8_t kAccelerometerI2CAddress = 0x1C;
const uint8_t kAccelerometerCtrlReg1   = 0x2A;
const uint8_t kAccelerometerXYZDataCFG = 0x0E;
const uint8_t kAccelerometerDataReg    = 0x00;
const uint8_t kAccelerometerOnCommand  = 0x01;
const uint8_t kAccelerometerOffCommand = 0x00;
const uint8_t kSleepTime_ms            = 2;
const uint8_t kSampleBufferSize        = 16;


Accelerometer::Accelerometer() : shutdown(false), samples() {
    auto *i2c_m = I2CManager::Get();
    i2c_m->SetSlaveAddress(kAccelerometerI2CAddress);

    ActivateAccelerometer();

    SetSensitivity(Accelerometer::Sensitivity::SENS_2G);

    worker_thread = std::thread(&Accelerometer::Worker, this);
}

Accelerometer::~Accelerometer() {
    shutdown = true;
    worker_thread.join();
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
    // Buffer for storing accelerometer reading.
    uint8_t buf[7];
    memset(buf, 0, sizeof(*buf) * 7);

    auto *i2c_m = I2CManager::Get();
    i2c_m->ReadFromRegister(kAccelerometerDataReg, (char *)buf, 7);

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
    auto *i2c_m = I2CManager::Get();
    i2c_m->WriteToRegister(kAccelerometerCtrlReg1, kAccelerometerOnCommand);
}

void Accelerometer::ShutdownAccelerometer() {
    auto *i2c_m = I2CManager::Get();
    i2c_m->WriteToRegister(kAccelerometerCtrlReg1, kAccelerometerOffCommand);
}

void Accelerometer::SetSensitivity(Accelerometer::Sensitivity sensitivity) {
    auto *i2c_m = I2CManager::Get();
    i2c_m->WriteToRegister(kAccelerometerXYZDataCFG, sensitivity);
}

} // earthquake_detection_unit

