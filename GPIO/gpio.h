#pragma once

#include <string>

namespace earthquake_detection_unit {

class GPIO {
public:
    enum PinValue {
        LOW,     /* 0 */
        HIGH,    /* 1 */
    };

    enum PinDirection {
        IN,      /* 0 */
        OUT      /* 1 */
    };

    enum PinEdge {
        NONE,    /* 0 */
        RISING,  /* 1 */
        FALLING, /* 2 */
        BOTH     /* 3 */
    };

    GPIO(int gpio_number);
    ~GPIO();

    // Writes `value` to .../gpioXX/value file.
    void SetValue(PinValue value);
    // Writes `direction` to .../gpioXX/direction file.
    void SetDirection(PinDirection direction);
    // Write `edge` to .../gpioXX/edge file.
    void SetEdge(PinEdge edge);

    // Returns the value file directory for this GPIO object.
    std::string GetValueFilePath() const;

private:
    // Exports `gpio_number` pin as GPIO.
    void ExportGPIOPin();
    // Unexports `gpio_number` pin.
    void UnexportGPIOPin();

    // Returns GPIO directory corresponding to `gpio_number`.
    std::string GetGPIODirectory() const;

    // Linux GPIO number.
    int gpio_number;
};

} // earthquake_detection_unit
