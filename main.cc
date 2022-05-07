#include <iostream>

#include <EarthquakeDetector/earthquake_detector.h>
#include <GPIO/export_file_manager.h>

int main(int argc, char **argv) {
    // Initialize singletons.
    earthquake_detection_unit::ExportFileManager::Initialize();

    auto *ed = new earthquake_detection_unit::EarthquakeDetector();
    while (true);
    delete ed;

    // Shutdown singletons.
    earthquake_detection_unit::ExportFileManager::Uninitialize();
    
    return 0;
}
