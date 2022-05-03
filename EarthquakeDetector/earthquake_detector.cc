
#include "earthquake_detector.h"

namespace earthquake_detection_unit {

EarthquakeDetector::EarthquakeDetector() : shutdown(false) {
    worker_thread = std::thread(&EarthquakeDetector::Worker, this);
}

EarthquakeDetector::~EarthquakeDetector() {
    shutdown = true;
}

void EarthquakeDetector::Worker() {
    while (!shutdown) {
        
    }
}

} // earthquake_detection_unit
