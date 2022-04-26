#include <atomic>
#include <iomanip>
#include <iostream>
#include <thread>

#include <Accelerometer/accelerometer.h>
#include <I2CManager/i2c_manager.h>

int main() {
    earthquake_detection_unit::I2CManager::Initialize();
    auto *a = new earthquake_detection_unit::Accelerometer();
    std::atomic<bool> stop_printing(false);

    // Lambda for printing accelerometer readings.
    auto thd_function = [&]() {
        while (!stop_printing) {
            auto v = a->GetReading();
            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
            std::cout << "mangitude: " << v.magnitude << std::endl << std::endl;
        }
    };

    // // Launch thread to print accelerometer readings.
    std::thread print_thread(thd_function);

    std::this_thread::sleep_for(std::chrono::seconds(15));

    // // Shutdown thread.
    stop_printing = true;
    print_thread.join();

    delete a;
    earthquake_detection_unit::I2CManager::Uninitialize();
    return 0;
}
