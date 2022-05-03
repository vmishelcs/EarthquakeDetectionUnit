#pragma once
#include <atomic>
#include <thread>

namespace earthquake_detection_unit {

class EarthquakeDetector {
public:
    EarthquakeDetector();
    ~EarthquakeDetector();

private:
    void Worker();

    // Signal to shutdown worker thread.
    std::atomic<bool> shutdown;
    // Worker thread member variable.
    std::thread worker_thread;
};

} // earthquake_detection_unit
