#include <iostream>

#include <EarthquakeDetector/earthquake_detector.h>

int main(int argc, char **argv) {
    auto earthquake_detector = new earthquake_detection_unit::EarthquakeDetector();

    std::cout << "Press `Enter` to quit.\n";
    std::cin.get();
    std::cout << "The earthquake detector will shut down after the next accelerometer time out.\n";
    
    delete earthquake_detector;

    return 0;
}
