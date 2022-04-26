#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <thread>

#include "gpio.h"
#include "export_file_manager.h"

namespace earthquake_detection_unit {

const std::string kExportFilePath   = "/sys/class/gpio/export";
const std::string kUnexportFilePath = "/sys/class/gpio/unexport";

GPIO::GPIO(int gpio_number) : gpio_number(gpio_number) {
    // Check if pin needs to be exported.
    DIR *dir = opendir(GetGPIODirectory().c_str());
    if (!dir) {
        auto *efm = ExportFileManager::Get();
        efm->ExportPin(gpio_number);
        closedir(dir);
    }
}

GPIO::~GPIO() {
    // Don't export as that drives some GPIO pins to HIGH.
    // auto *efm = ExportFileManager::Get();
    // efm->UnexportPin(gpio_number);
}

void GPIO::WriteToGPIOValueFile(GPIO::PinValue value) {
    std::string value_file_path = GetGPIODirectory() + "value";
    std::ofstream value_file(value_file_path);
    if (value_file.fail()) {
        std::cerr << "ERROR: failed to open " << value_file_path;
        std::cerr << " in GPIO::WriteToGPIOValueFile." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    std::string pin_value_str = std::to_string(value);
    value_file << pin_value_str;
    value_file.close();
}

void GPIO::WriteToGPIODirectionFile(GPIO::PinDirection direction) {
    std::string direction_file_path = GetGPIODirectory() + "direction";
    std::ofstream direction_file(direction_file_path);
    if (direction_file.fail()) {
        std::cerr << "ERROR: failed to open " << direction_file_path;
        std::cerr << " in GPIO::WriteToGPIODirectionFile." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    std::string direction_str = direction == GPIO::PinDirection::IN ? 
                                             std::string("in") :
                                             std::string("out");
    direction_file << direction_str;
    direction_file.close();
}

void GPIO::ExportGPIOPin() {
    std::ofstream export_file(kExportFilePath);
    if (export_file.fail()) {
        std::cerr << "ERROR: failed to open " << kExportFilePath;
        std::cerr << " in GPIO::ExportGPIOPin." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    std::string gpio_number_str = std::to_string(gpio_number);
    export_file << gpio_number_str;
    export_file.close();

    // Sleep for 300 ms after exporting a pin.
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void GPIO::UnexportGPIOPin() {
    std::ofstream unexport_file(kUnexportFilePath);
    if (unexport_file.fail()) {
        std::cerr << "ERROR: failed to open " << kExportFilePath;
        std::cerr << " in GPIO::UnexportGPIOPin." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    std::string gpio_number_str = std::to_string(gpio_number);
    unexport_file << gpio_number_str;
    unexport_file.close();
}

std::string GPIO::GetGPIODirectory() {
    std::string base = "/sys/class/gpio/gpio";
    std::string gpio_number_str = std::to_string(gpio_number);
    std::string result = base + gpio_number_str + "/";
    return result;
}

} // earthquake_detection_unit
