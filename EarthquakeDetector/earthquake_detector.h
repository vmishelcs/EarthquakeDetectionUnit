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
    void Worker();
    void AccelerometerMonitor();

    // Blocking function to check if accelerometer is detecting significant readings.
    void AccelerometerTimeout();

    // Worker thread member variable.
    std::thread worker_thread;
    // Signal to shutdown worker thread.
    std::atomic<bool> shutdown;
    // Thread for monitoring accelerometer readings.
    std::thread accelerometer_monitor_thread;
    // Signal to shutdown accelerometer monitoring thread.
    std::atomic<bool> shutdown_accelerometer_monitor;

    Accelerometer *accelerometer;
    DigitDisplay *digit_display;
    VibrationSensor *vibration_sensor;
};

} // earthquake_detection_unit
