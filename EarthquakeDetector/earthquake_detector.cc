#include <iostream>

#include "earthquake_detector.h"

namespace earthquake_detection_unit {

const double kScaleValue1Threshold = 0.144;
const double kScaleValue2Threshold = 0.281;
const double kScaleValue3Threshold = 0.418;
const double kScaleValue4Threshold = 0.555;
const double kScaleValue5Threshold = 0.692;
const double kScaleValue6Threshold = 0.829;
const double kScaleValue7Threshold = 0.966;
const double kScaleValue8Threshold = 1.103;
const double kScaleValue9Threshold = 1.24;
const int kAccelerometerTimeoutTotal_ms = 10000;
const int kAccelerometerTimeoutPeriod_ms = 500;
const int kAccelerometerTimeoutNumPeriods = kAccelerometerTimeoutTotal_ms / kAccelerometerTimeoutPeriod_ms;

EarthquakeDetector::EarthquakeDetector() {
    std::atomic<bool> shutdown(false);
    std::atomic<bool> shutdown_accelerometer_monitor(false);

    // Initialize digit display.
    digit_display = new DigitDisplay();
    digit_display->SetDigit(5);

    worker_thread = std::thread(&EarthquakeDetector::Worker, this);
}

EarthquakeDetector::~EarthquakeDetector() {
    shutdown = true;
    worker_thread.join();

    delete digit_display;
}

void EarthquakeDetector::Worker() {
    while (!shutdown) {
        std::cout << "Launching vibration sensor to listen for a vibration." << std::endl;
        // First, wait for a vibration.
        vibration_sensor = new VibrationSensor();
        vibration_sensor->WaitForVibration();
        delete vibration_sensor;

        std::cout << "Vibration detected -- vibration sensor shutdown, launching accelerometer." << std::endl;

        // After detecting a vibration, launch accelerometer.
        accelerometer = new Accelerometer();
        shutdown_accelerometer_monitor.store(false, std::memory_order_relaxed);
        accelerometer_monitor_thread = std::thread(&EarthquakeDetector::AccelerometerMonitor, this);

        // Keep checking if we are still detecting significant shaking via the accelerometer.
        AccelerometerTimeout();

        std::cout << "Shutting down accelerometer." << std::endl;
        // Shutdown accelerometer for lack of activity.
        shutdown_accelerometer_monitor.store(true, std::memory_order_relaxed);
        accelerometer_monitor_thread.join();
        delete accelerometer;
    }
}

void EarthquakeDetector::AccelerometerMonitor() {
    while (!shutdown_accelerometer_monitor) {
        // double acc_reading = accelerometer->GetHighestReading();
        
    }
}

void EarthquakeDetector::AccelerometerTimeout() {
    // This function returns if we obtain kAccelerometerTimeoutNumPeriods
    // consecutive low readings from the accelerometer.
    int consecutive_readings = 0;
    while (consecutive_readings != kAccelerometerTimeoutNumPeriods) {
        if (accelerometer->GetCurrentReading() >= kScaleValue1Threshold) {
            consecutive_readings = 0;
        }
        else {
            ++consecutive_readings;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(kAccelerometerTimeoutPeriod_ms));
    }
}

} // earthquake_detection_unit
