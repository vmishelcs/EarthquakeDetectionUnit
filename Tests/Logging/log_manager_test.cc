#include <chrono>
#include <iostream>
#include <thread>

#include <Logging/log_manager.h>

int main() {
    earthquake_detection_unit::LogManager::Initialize();
    auto *lm = earthquake_detection_unit::LogManager::Get();

    for (uint32_t i = 0; i < 10; ++i) {
        std::cout << "Logging " << i << std::endl;
        lm->LogRecord(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    earthquake_detection_unit::LogManager::Uninitialize();
}
