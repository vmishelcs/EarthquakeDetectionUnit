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

    void WriteToGPIOValueFile(PinValue value);
    void WriteToGPIODirectionFile(PinDirection direction);

private:
    void ExportGPIOPin();
    void UnexportGPIOPin();

    std::string GetGPIODirectory();

    // Linux GPIO number.
    int gpio_number;
};

} // earthquake_detection_unit
