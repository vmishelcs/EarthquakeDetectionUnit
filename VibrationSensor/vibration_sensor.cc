#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

#include "vibration_sensor.h"

namespace earthquake_detection_unit {

const int kVibrationSensorGPIONumber = 67;

VibrationSensor::VibrationSensor() : pin(kVibrationSensorGPIONumber) {
    pin.SetDirection(GPIO::PinDirection::IN);
    pin.SetEdge(GPIO::PinEdge::RISING);
}

VibrationSensor::~VibrationSensor() {
    // Nothing here.
}

void VibrationSensor::WaitForVibration() {
    // Create a new epoll instance.
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "ERROR: failed to create new epoll instance ";
        std::cerr << " in VibrationSensor::WaitForVibration." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    // Get GPIO pin value file descriptor.
    int gpio_fd = open(pin.GetValueFilePath().c_str(), O_RDONLY | O_NONBLOCK);
    if (gpio_fd == -1) {
        std::cerr << "ERROR: failed to open " << pin.GetValueFilePath();
        std::cerr << " in VibrationSensor::WaitForVibration." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        close(epoll_fd);
        std::_Exit(EXIT_FAILURE);
    }

    struct epoll_event ep;
    ep.events = EPOLLIN | EPOLLET;
    ep.data.fd = gpio_fd;

    // Add epoll event.
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, gpio_fd, &ep) == -1) {
        std::cerr << "ERROR: failed to add epoll event";
        std::cerr << " in VibrationSensor::WaitForVibration." << std::endl;
        std::cerr << "errno: " << strerror(errno) << std::endl;
        close(epoll_fd);
        close(gpio_fd);
        std::_Exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 2; ++i) {
        if (epoll_wait(epoll_fd, &ep, 1, -1) == -1) {
            std::cerr << "ERROR: epoll_wait failed";
            std::cerr << " in VibrationSensor::WaitForVibration." << std::endl;
            std::cerr << "errno: " << strerror(errno) << std::endl;
            close(epoll_fd);
            close(gpio_fd);
            std::_Exit(EXIT_FAILURE);
        }
    }

    close(epoll_fd);
    close(gpio_fd);
}


} // earthquake_detection_unit

