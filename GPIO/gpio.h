#pragma once

#include <string>

namespace earthquake_detection_unit {

class GPIO {
public:
    enum PinValue {
        LOW,  /* 0 */
        HIGH, /* 1 */
    };

    enum PinDirection {
        IN,   /* 0 */
        OUT   /* 1 */
    };

    GPIO(int gpio_number);
    ~GPIO();

    // Writes `value` to .../gpioXX/value file.
    void WriteToGPIOValueFile(PinValue value);
    // Writes `direction` to .../gpioXX/direction file.
    void WriteToGPIODirectionFile(PinDirection direction);

    // Obtains the value file directory for this GPIO object.
    std::string GetValueFilePath();

private:
    // Exports `gpio_number` pin as GPIO.
    void ExportGPIOPin();
    // Unexports `gpio_number` pin.
    void UnexportGPIOPin();

    std::string GetGPIODirectory() const;

    // Linux GPIO number.
    int gpio_number;
};

} // earthquake_detection_unit
