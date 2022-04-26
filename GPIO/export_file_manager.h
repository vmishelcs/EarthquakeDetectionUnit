#pragma once

#include <mutex>

namespace earthquake_detection_unit {

class ExportFileManager {
private:
    static ExportFileManager *instance;

public:
    inline static void Initialize() { instance = new ExportFileManager(); }
    inline static void Uninitialize() { delete instance; }
    inline static ExportFileManager *Get() { return instance; }

    ExportFileManager(ExportFileManager const &) = delete;
    void operator=(ExportFileManager const &) = delete;

    void ExportPin(int gpio_number);
    void UnexportPin(int gpio_number);

private:
    ExportFileManager();
    ~ExportFileManager();

    // File descriptors for export and unexport files.
    int export_file_fd;
    int unexport_file_fd;

    // Mutexes for concurrent file access.
    std::mutex export_file_mtx;
    std::mutex unexport_file_mtx;
};

} // earthquake_detection_unit
