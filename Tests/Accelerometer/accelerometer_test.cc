#include <atomic>
#include <iomanip>
#include <iostream>
#include <thread>

#include <Accelerometer/accelerometer.h>
#include <I2CManager/i2c_manager.h>

int main() {
    earthquake_detection_unit::I2CManager::Initialize();
    auto *a = new earthquake_detection_unit::Accelerometer();
    std::atomic<bool> begin_printing(false);
    std::atomic<bool> stop_printing(false);

    // Lambda for printing accelerometer readings.
    auto thd_function = [&]() {
        while (!begin_printing);
        std::cout << std::fixed;
        std::cout << std::setprecision(2);
        std::cout << "Current reading: " << a->GetCurrentReading();
        while (!stop_printing) {
            std::cout << "\b\b\b\b";
            std::cout << a->GetCurrentReading();
        }
        std::cout << "\rShutting down...";
        std::cout << std::endl;
    };

    // Launch thread to print accelerometer readings.
    std::thread print_thread(thd_function);

    std::cout << "Press `Enter` to quit." << std::endl;
    begin_printing = true;
    std::cin.get();

    // Shutdown thread.
    stop_printing = true;
    print_thread.join();

    delete a;
    earthquake_detection_unit::I2CManager::Uninitialize();
    return 0;
}
