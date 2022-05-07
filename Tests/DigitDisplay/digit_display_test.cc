#include <iostream>

#include <GPIO/export_file_manager.h>
#include <DigitDisplay/digit_display.h>

int main() {
    earthquake_detection_unit::ExportFileManager::Initialize();
    auto *dd = new earthquake_detection_unit::DigitDisplay();

    for (int i = 0; i < 10; ++i) {
        dd->SetDigit(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    dd->FlashDisplay();

    delete dd;
    earthquake_detection_unit::ExportFileManager::Uninitialize();
    return 0;
}
