# Earthquake Detection Unit


## Installing prerequisites.

Begin by installing the necessary compilation tools.
```shell
$ sudo apt-get install build-essential
$ sudo apt-get install gcc-arm-linux-gnueabihf
$ sudo apt-get install g++-arm-linux-gnueabihf
$ sudo apt-get install cmake
```
You may need to edit lines 4 and 5 of the `edu_toolchain.cmake` file to specify the paths of the C/C++ cross compilers.


## Building the project.

In the EDU source code directory, create a `build` directory and change to it.
```shell
$ mkdir build && cd build
```
Set the source and build directory for CMake.
```shell
$ cmake ..
```
Finally, build the project.
```shell
$ make
```
The program executable is titled `edu`, located in the `build` directory. There are several tests in the `build/Tests` directory used to debug various components of the project.

