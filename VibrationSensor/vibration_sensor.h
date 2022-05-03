#pragma once

#include <GPIO/gpio.h>

namespace earthquake_detection_unit {

class VibrationSensor {
public:
    VibrationSensor();
    ~VibrationSensor();

    // Blocks thread until a vibration is detected.
    void WaitForVibration();

private:
    // GPIO pin tied to the vibration sensor.
    GPIO pin;
};

} // earthquake_detection_unit
