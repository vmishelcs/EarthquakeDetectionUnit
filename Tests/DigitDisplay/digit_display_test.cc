#include <iostream>

#include <GPIO/export_file_manager.h>
#include <DigitDisplay/digit_display.h>
#include <I2CManager/i2c_manager.h>

int main() {
    earthquake_detection_unit::ExportFileManager::Initialize();
    earthquake_detection_unit::I2CManager::Initialize();
    auto *dd = new earthquake_detection_unit::DigitDisplay();

    for (int i = 0; i < 10; ++i) {
        dd->SetDigit(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    delete dd;
    earthquake_detection_unit::I2CManager::Uninitialize();
    return 0;
}
