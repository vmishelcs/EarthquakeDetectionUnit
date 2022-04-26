#pragma once

#include <atomic>
#include <cstdint>
#include <thread>

#include <GPIO/gpio.h>

namespace earthquake_detection_unit {

class DigitDisplay {
public:
    DigitDisplay();
    ~DigitDisplay();

    void SetDigit(uint8_t digit);

private:

    // Value which we are currently displaying.
    uint8_t current_digit;
    // GPIO object corresponding to our digit display.
    GPIO left_digit_gpio;
    GPIO right_digit_gpio;
};

} // earthquake_detection_unit
