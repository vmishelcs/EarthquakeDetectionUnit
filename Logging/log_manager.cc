#include <cerrno>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#include "log_manager.h"

namespace earthquake_detection_unit {

const int kMaxUsernameLength       = 255;
const int kTimeAndDateBufferLength = 32;
const int kLogEntryLength          = 52;
const int kLogFieldLength          = 17; // 16 + 1 for null char

LogManager *LogManager::instance = nullptr;

LogManager::LogManager() {
    // Obtain username.
    char username[kMaxUsernameLength] = { 0 };
    if (getlogin_r(username, kMaxUsernameLength) != 0) {
        std::cerr << "ERROR: failed to obtain username";
        std::cerr << " in LogManager constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    // Obtain user's home directory.
    std::string home_dir = "/home/" + std::string(username) + "/";

    // Generate log file directory.
    std::string log_file_dir = home_dir + "edu-logs/";
    
    // Check if log file directory already exists.
    DIR *dir = opendir(log_file_dir.c_str());
    if (!dir) {
        // If not, make it.
        if (mkdir(log_file_dir.c_str(), S_IRWXU) != 0) {
            std::cerr << "ERROR: failed to create logging directory";
            std::cerr << " in LogManager constructor." << std::endl;
            std::cerr << "errno: " << std::strerror(errno) << std::endl;
            std::_Exit(EXIT_FAILURE);
        }
    }
    closedir(dir);

    // Next step is to generate a unique file name.
    // We use the following convention:
    //     edu__MM-DD-YYYY__HH-MM-SS.log
    std::string log_file_name = "edu__";
    char time_and_date_buffer[kTimeAndDateBufferLength] = { 0 };
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_and_date_buffer, kTimeAndDateBufferLength, "%m-%d-%G__%H-%M-%S", time_info);
    log_file_name += std::string(time_and_date_buffer) + ".log";

    // Generate the log file path.
    std::string log_file_path = log_file_dir + log_file_name;

    // Create log file.
    log_file_fd = open(log_file_path.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (log_file_fd == -1) {
        std::cerr << "ERROR: failed to create log file";
        std::cerr << " in LogManager constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
        std::_Exit(EXIT_FAILURE);
    }

    // Write the start of the file.
    std::string file_start = " DATE           | TIME           | MAGNITUDE      \n";
    file_start += std::string(kLogEntryLength - 2, '-') + "\n";
    ssize_t bytes_written = write(log_file_fd, file_start.c_str(), file_start.length() + 1);
    if (bytes_written != static_cast<ssize_t>(file_start.length() + 1)) {
        std::cerr << "WARNING: wrote ";
        std::cerr << bytes_written;
        std::cerr << " bytes to log file";
        std::cerr << " in LogManager constructor." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
    }
}

LogManager::~LogManager() {
    // Close log file.
    close(log_file_fd);
}

void LogManager::LogRecord(unsigned int magnitude) {
    char date_buffer[kLogFieldLength] = { 0 };
    char time_buffer[kLogFieldLength] = { 0 };
    char magnitude_buffer[kLogFieldLength] = { 0 };

    // Obtain current time.
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);

    // Fill date field.
    strftime(date_buffer, kLogFieldLength, " %m.%d.%G     ", time_info);
    std::string log_str(date_buffer);
    // Fill time field.
    strftime(time_buffer, kLogFieldLength, " %H:%M:%S       ", time_info);
    log_str += "|" + std::string(time_buffer);
    // Fill magnitude field.
    snprintf(magnitude_buffer, kLogFieldLength, " %d              ", magnitude);
    log_str += "|" + std::string(magnitude_buffer) + "\n";

    // Write to log file.
    ssize_t bytes_written = write(log_file_fd, log_str.c_str(), log_str.length() + 1);
    if (bytes_written != static_cast<ssize_t>(log_str.length() + 1)) {
        std::cerr << "WARNING: wrote ";
        std::cerr << bytes_written;
        std::cerr << " bytes to log file";
        std::cerr << " in LogManager::LogRecord." << std::endl;
        std::cerr << "errno: " << std::strerror(errno) << std::endl;
    }
}

} // earthquake_detection_unit
