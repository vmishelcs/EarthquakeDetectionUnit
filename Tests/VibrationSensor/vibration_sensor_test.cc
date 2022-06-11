#include <atomic>
#include <iostream>
#include <thread>

#include <GPIO/export_file_manager.h>
#include <VibrationSensor/vibration_sensor.h>

int main() {
    earthquake_detection_unit::ExportFileManager::Initialize();
    auto *v = new earthquake_detection_unit::VibrationSensor();
    std::atomic<bool> stop_thread(false);

    // Lambda for printing detecting vibrations.
    auto thd_function = [&]() {
        while (!stop_thread) {
            v->WaitForVibration();
            std::cout << "Vibration detected." << std::endl;
        }
    };

    // Launch thread to print accelerometer readings.
    std::thread vibration_thread(thd_function);

    std::cout << "Press `Enter` to quit." << std::endl;
    std::cin.get();

    // Shut down thread.
    stop_thread = true;
    vibration_thread.join();

    delete v;
    earthquake_detection_unit::ExportFileManager::Uninitialize();
    return 0;
}
