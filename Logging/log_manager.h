#pragma once

namespace earthquake_detection_unit {

class LogManager {
private:
    static LogManager *instance;

public:
    inline static void Initialize() { instance = new LogManager(); }
    inline static void Uninitialize () { delete instance; }
    inline static LogManager *Get() { return instance; }

    LogManager(LogManager const &) = delete;
    void operator=(LogManager const &) = delete;

    void LogRecord(unsigned int magnitude);

private:
    LogManager();
    ~LogManager();

    // File descriptor for the logging file.
    int log_file_fd;
};

} // earthquake_detection_unit
