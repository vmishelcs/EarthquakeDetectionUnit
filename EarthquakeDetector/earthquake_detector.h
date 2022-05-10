#pragma once

#include <atomic>
#include <thread>

#include <Accelerometer/accelerometer.h>
#include <DigitDisplay/digit_display.h>
#include <VibrationSensor/vibration_sensor.h>

namespace earthquake_detection_unit {

class EarthquakeDetector {
public:
    EarthquakeDetector();
    ~EarthquakeDetector();

private:
    // Worker thread for continuous earthquake monitoring.
    void Worker();

    // Monitors accelerometer readings. This function returns after a certain 
    // number of consecutive low readings.
    void AccelerometerMonitor();

    // Displays current magnitude on the digit display.
    void DisplayMagnitude();

    // Worker thread member variable.
    std::thread worker_thread;
    // Signal to shut down worker thread.
    std::atomic<bool> shutdown;

    // Pointers to devices.
    Accelerometer *accelerometer;
    DigitDisplay *digit_display;
    VibrationSensor *vibration_sensor;
};

} // earthquake_detection_unit
