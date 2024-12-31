# Minecraft: Nintendo Switch Edition Modding
A WIP toolkit for modding Minecraft: Nintendo Switch Edition (the legacy version). This project is based on [Marshmallow](https://github.com/odyssey-modding/marshmallow).

This project is in an extremely WIP state from when I last worked on it. It features:
- A port of Minecraft beta's terrain and terrain features
- - Expect the nether to spawn overworld mobs
- Beta biomes
- Beta leaf lighting
- Debug menu based on the F3 menu in Java Edition
- A non-functional mouse and keyboard implementation
- A semi-functional packet dumper

## Setup
The only supported host platforms are Windows and Linux.

Dependencies: [CMake](https://cmake.org/)(>=v3.21) and [Ninja](https://ninja-build.org/).  
Both should be available from your package manager, be it `winget`, `apt`, `pacman` or anything else.

To configure the CMake project, run this command. See [definitions](#CMake definition args) for flags to add on.
```shell
cmake -S . -B build -G Ninja --toolchain=cmake/toolchain.cmake
```
And to build using Ninja, simply run:
```shell
ninja -C build
```
Any other target generators are untested and unsupported.

### CMake definition arguments:
- `-DFTP_IP=XXX.XXX.XXX.XXX`
  - optional, will enable FTP deployment builds if specified 
  - your switch's IP for deployment with the `subsdk9_deploy_ftp` target
- `-DFTP_PORT=5000`, `-DFTP_USERNAME=anonymous`, `-DFTP_PASSWORD=anonymous`
  - optional, uses defaults above
  - your switch's FTP port, username, and password
- `-DFTP_TITLE_ID=0100000000010000`
  - optional, uses defaults above
  - what title id to use when placing deployments into `/atmosphere/contents/FTP_TITLE_ID/exefs/`

## Credits
- [Marshmallow](https://github.com/odyssey-modding/marshmallow)
