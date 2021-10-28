# GBLib
`GBLib` is GameBoy emulator written as a portable C++ library. Easily inject platform specific dependencies to run anywhere.

For an example on how to build an application on top of `GBLib` refer to [GBSdl](https://github.com/David-Parker/GBSdl).

## How to Build
In the root directory run cmake.
`cmake .`

CMake will generate platform specific build tools, e.g. a `Makefile` on Linux.

All source files reside in the `src` directory and header files reside in the `include` directory.

## License
`GBLib` is released under the MIT license.

## Testing
To run unit tests, use the test project listed under the `test` folder. To run Blargg's GB Test Roms, load any of the roms under `rom\gb-test-roms` sub module.

## Media

![image](images/pokemon-start.jpg)
![image](images/pokemon.jpg)
![image](images/tetris.jpg)
![image](images/hi-score.jpg)
![image](images/dr-mario.jpg)
![image](images/dr-mario-play.jpg)
![image](images/blargg-cpu.jpg)