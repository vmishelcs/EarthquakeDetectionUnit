#include <cassert>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>

#include "digit_display.h"
#include <I2CControl/i2c_control.h>

namespace earthquake_detection_unit {

const int kLeftDigitGPIONumber        = 61;
const int kRightDigitGPIONumber       = 44;
const uint8_t kDigitDisplayI2CAddress = 0x20;
const uint8_t kLowerDigitRegister     = 0x14;
const uint8_t kUpperDigitRegister     = 0x15;

DigitDisplay::DigitDisplay() : left_digit_gpio(kLeftDigitGPIONumber),
                               right_digit_gpio(kRightDigitGPIONumber)
{
    i2c_c = new I2CControl();
    i2c_c->SetSlaveAddress(kDigitDisplayI2CAddress);

    // Set direction of both 8-bit ports on the I2C GPIO extender to be outputs.
    i2c_c->WriteToRegister(0x00, 0x00);
    i2c_c->WriteToRegister(0x01, 0x00);

    // Set GPIO pin to output.
    left_digit_gpio.SetDirection(GPIO::PinDirection::OUT);
    right_digit_gpio.SetDirection(GPIO::PinDirection::OUT);

    // Initialize to 0.
    current_digit = 0;
    i2c_c->WriteToRegister(kLowerDigitRegister, 0xA3);
    i2c_c->WriteToRegister(kUpperDigitRegister, 0x96);    

    // Turn display on (just the right digit).
    left_digit_gpio.SetValue(GPIO::PinValue::LOW);
    right_digit_gpio.SetValue(GPIO::PinValue::HIGH);
}

DigitDisplay::~DigitDisplay() {
    // Turn off digit display.
    left_digit_gpio.SetValue(GPIO::PinValue::LOW);
    right_digit_gpio.SetValue(GPIO::PinValue::LOW);

    delete i2c_c;
}

void DigitDisplay::SetDigit(unsigned int digit) {
    if (digit != current_digit) {
        assert(digit < 10);
        current_digit = digit;

        uint8_t lower_bits;
        uint8_t upper_bits;
        switch (digit) {
            case 1:
                lower_bits = 0x80;
                upper_bits = 0x12;
                break;
            
            case 2:
                lower_bits = 0x31;
                upper_bits = 0x0E;
                break;

            case 3:
                lower_bits = 0xB0;
                upper_bits = 0x06;
                break;

            case 4:
                lower_bits = 0x90;
                upper_bits = 0x8A;
                break;

            case 5:
                lower_bits = 0xB0;
                upper_bits = 0x8C;
                break;

            case 6:
                lower_bits = 0xB1;
                upper_bits = 0x8C;
                break;

            case 7:
                lower_bits = 0x04;
                upper_bits = 0x14;
                break;

            case 8:
                lower_bits = 0xB1;
                upper_bits = 0x8E;
                break;

            case 9:
                lower_bits = 0x90;
                upper_bits = 0x8E;
                break;

            default:
                lower_bits = 0xA3;
                upper_bits = 0x96;
        }

        // Drive lower 8-bits.
        i2c_c->WriteToRegister(kLowerDigitRegister, lower_bits);
        // Drive upper 8-bits.
        i2c_c->WriteToRegister(kUpperDigitRegister, upper_bits);   
    }
}

void DigitDisplay::FlashDisplay() {
    for (int i = 0; i < 5; ++i) {
        // Turn display off.
        right_digit_gpio.SetValue(GPIO::PinValue::LOW);

        // Wait 200 ms.
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // Turn display on.
        right_digit_gpio.SetValue(GPIO::PinValue::HIGH);

        // Wait 200 ms.
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

} // earthquake_detection_unit
