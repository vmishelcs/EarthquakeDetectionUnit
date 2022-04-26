cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE "/home/vmishel/earthquake_detection_unit/edu_toolchain.cmake")

project("Earthquake Detection Unit")

# No in-source build.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(Accelerometer)
add_subdirectory(DigitDisplay)
add_subdirectory(EarthquakeDetector)
add_subdirectory(GPIO)
add_subdirectory(I2CManager)

add_subdirectory(Tests)
