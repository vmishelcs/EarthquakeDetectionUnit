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

    // Sets the digit to be displayed.
    void SetDigit(uint8_t digit);

private:

    // Value which we are currently displaying.
    uint8_t current_digit;
    // GPIO objects corresponding to the digit display.
    GPIO left_digit_gpio;
    GPIO right_digit_gpio;
};

} // earthquake_detection_unit
