#include <iostream>
#include <string>

#include <EarthquakeDetector/earthquake_detector.h>
#include <GPIO/export_file_manager.h>
#include <Logging/log_manager.h>

int main(int argc, char **argv) {
    earthquake_detection_unit::ExportFileManager::Initialize();
    earthquake_detection_unit::LogManager::Initialize();
    
    auto *ed = new earthquake_detection_unit::EarthquakeDetector();

    std::cout << "Press `Enter` to quit." << std::endl;
    std::cin.get();
    std::cout << "The earthquake detector will shut down after the next accelerometer time out." << std::endl;

    delete ed;

    earthquake_detection_unit::LogManager::Uninitialize();
    earthquake_detection_unit::ExportFileManager::Uninitialize();

    std::cout << "Shutting down..." << std::endl;
    return 0;
}
