#include <iostream>

#include "earthquake_detector.h"
#include <GPIO/export_file_manager.h>
#include <Logging/log_manager.h>

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
const int kAccelerometerSamplePeriod_ms = 100;
const int kAccelerometerTimeoutNumPeriods = kAccelerometerTimeoutTotal_ms / kAccelerometerSamplePeriod_ms;

EarthquakeDetector::EarthquakeDetector() : shutdown(false) {
    // Initialize necessary utilities.
    ExportFileManager::Initialize();
    LogManager::Initialize();

    // Initialize digit display.
    digit_display = new DigitDisplay();

    worker_thread = std::thread(&EarthquakeDetector::Worker, this);
}

EarthquakeDetector::~EarthquakeDetector() {
    shutdown.store(true, std::memory_order_relaxed);
    worker_thread.join();

    // Shut down digit display.
    delete digit_display;

    // Shut down utilities.
    LogManager::Uninitialize();
    ExportFileManager::Uninitialize();
}

void EarthquakeDetector::Worker() {
    while (!shutdown) {
        // First, wait for a vibration.
        std::cout << "\t<EarthquakeDetector> ";
        std::cout << "Launching vibration sensor to listen for a vibration.\n";
        vibration_sensor = new VibrationSensor();
        vibration_sensor->WaitForVibration();
        delete vibration_sensor;

        // After detecting a vibration, launch accelerometer.
        std::cout << "\t<EarthquakeDetector> ";
        std::cout << "Vibration detected.\n";
        std::cout << "\t<EarthquakeDetector> ";
        std::cout << "Shutting down vibration sensor and launching accelerometer.\n";
        accelerometer = new Accelerometer();

        // Monitor accelerometer readings.
        AccelerometerMonitor();

        // Flash magnitude.
        digit_display->FlashDisplay();
        // Reset digit display to display 0.
        digit_display->SetDigit(0);

        // Shut down accelerometer for lack of activity.
        std::cout << "\t<EarthquakeDetector> ";
        std::cout << "Shutting down accelerometer due to inactivity.\n";
        delete accelerometer;
    }
}

void EarthquakeDetector::AccelerometerMonitor() {
    // This function returns if we obtain kAccelerometerTimeoutNumPeriods
    // consecutive low readings from the accelerometer.
    int consecutive_readings = 0;
    while (consecutive_readings != kAccelerometerTimeoutNumPeriods) {
        DisplayMagnitude();
        if (accelerometer->GetCurrentReading() >= kScaleValue1Threshold) {
            consecutive_readings = 0;
        }
        else {
            ++consecutive_readings;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(kAccelerometerSamplePeriod_ms));
    }

    // Log the magnitude before returning.
    auto *log_manager = LogManager::Get();
    log_manager->LogRecord(digit_display->GetCurrentDigit());
}

void EarthquakeDetector::DisplayMagnitude() {
    double acc_reading = accelerometer->GetHighestReading();

    unsigned int digit_to_display = 0;
    if (acc_reading >= kScaleValue9Threshold) {
        digit_to_display = 9;
    }
    else if (acc_reading >= kScaleValue8Threshold) {
        digit_to_display = 8;
    }
    else if (acc_reading >= kScaleValue7Threshold) {
        digit_to_display = 7;
    }
    else if (acc_reading >= kScaleValue6Threshold) {
        digit_to_display = 6;
    }
    else if (acc_reading >= kScaleValue5Threshold) {
        digit_to_display = 5;
    }
    else if (acc_reading >= kScaleValue4Threshold) {
        digit_to_display = 4;
    }
    else if (acc_reading >= kScaleValue3Threshold) {
        digit_to_display = 3;
    }
    else if (acc_reading >= kScaleValue2Threshold) {
        digit_to_display = 2;
    }
    else if (acc_reading >= kScaleValue1Threshold) {
        digit_to_display = 1;
    }

    digit_display->SetDigit(digit_to_display);
}

} // earthquake_detection_unit
