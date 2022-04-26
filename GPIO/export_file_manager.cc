#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "export_file_manager.h"

namespace earthquake_detection_unit {

const std::string kExportFilePath   = "/sys/class/gpio/export";
const std::string kUnexportFilePath = "/sys/class/gpio/unexport";

ExportFileManager *ExportFileManager::instance = nullptr;

ExportFileManager::ExportFileManager() {
    export_file_fd = open(kExportFilePath.c_str(), O_RDWR);
    if (export_file_fd == -1) {
        std::cerr << "ERROR: unable to open " << kExportFilePath;
        std::cerr << " in ExportFileManager constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    unexport_file_fd = open(kUnexportFilePath.c_str(), O_RDWR);
    if (unexport_file_fd == -1) {
        std::cerr << "ERROR: failed to open " << kExportFilePath;
        std::cerr << " in ExportFileManager constructor." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }
}

ExportFileManager::~ExportFileManager() {
    close(export_file_fd);
    close(unexport_file_fd);
}

void ExportFileManager::ExportPin(int gpio_number) {
    export_file_mtx.lock();

    std::string gpio_number_str = std::to_string(gpio_number);
    if (write(export_file_fd, gpio_number_str.c_str(), gpio_number_str.size()) != (int)gpio_number_str.size()) {
        std::cerr << "ERROR: failed to export gpio" << gpio_number;
        std::cerr << " in ExportFileManager::ExportPin." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    export_file_mtx.unlock();

    // Sleep for 300 ms after exporting a pin.
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void ExportFileManager::UnexportPin(int gpio_number) {
    unexport_file_mtx.lock();

    std::string gpio_number_str = std::to_string(gpio_number);
    if (write(unexport_file_fd, gpio_number_str.c_str(), gpio_number_str.size()) != (int)gpio_number_str.size()) {
        std::cerr << "ERROR: failed to export gpio" << gpio_number;
        std::cerr << " in ExportFileManager::UnexportPin." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    unexport_file_mtx.unlock();
}

} // earthquake_detection_unit
