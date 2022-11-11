# Earthquake Detection Unit


## About the project.

Earthquake Detection Unit (EDU) is a simple earthquake detector for the BeagleBone Green equipped with the Zen cape and the SW-420 vibration sensor. EDU attempts to approximately map the vibrations it detects to the Richter scale. The computed magnitude is then displayed on the 14-segment digit display. The digit display is reset back to zero after a prolonged period of inactivity, and the previously displayed value is logged in the `~/edu-logs/` directory. The logging files follow the naming convention `edu__MM-DD-YYYY__HH-MM-SS.log`, where the date and time specified in the file name is the date and time at which EDU was launched.


## Installing prerequisites.

Begin by installing the necessary compilation tools.

```shell
$ sudo apt-get install build-essential
$ sudo apt-get install gcc-arm-linux-gnueabihf
$ sudo apt-get install g++-arm-linux-gnueabihf
$ sudo apt-get install binutils-arm-linux-gnueabihf
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


## Running the code.

In order for the BeagleBone to obtain the compiled binaries and run them, we set up a Network File Sharing (NFS) server using Ethernet over USB. Start by installing the NFS server program on the host computer.

``` shell
$ sudo apt-get install portmap nfs-kernel-server
```

Next, we configure the server by appending a line to the `/etc/exports` file by specifying the build directory, the USB IP address and the netmask. In my case, since my EDU project folder is in the home directory, I would write the following.

```
/home/vmishel/EarthquakeDetectionUnit/build 192.168.7.0/255.255.255.252(rw,sync,no_subtree_check)
```

Restart the server so that our changes to `/etc/exports` can take effect.

```shell
$ sudo exportfs -rav
$ sudo /etc/init.d/nfs-kernel-server restart
```

Check to make sure that the correct directory is exported.

```shell
$ showmount -e
```

Now we must mount the NFS server on the BeagleBone. First create a mount point.

```shell
$ sudo mkdir /mnt/edu-remote-build
```