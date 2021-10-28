cmake_minimum_required(VERSION 3.9)

project(GBLib VERSION 0.0.1 DESCRIPTION "Nintendo GameBoy Emulator as a shared library.")

add_library(GBLib SHARED
src/ALUHelpers.cpp
src/Address.cpp
src/BGMap.cpp
src/CartridgeHeader.cpp
src/Cpu.cpp
src/GameBoy.cpp
src/GraphicsManager.cpp
src/InterruptController.cpp
src/JoypadController.cpp
src/MBC.cpp
src/Memory.cpp
src/PixelProcessingUnit.cpp
src/RAM.cpp
src/ROM.cpp
src/SerialTransferController.cpp
src/SoundController.cpp
src/Tile.cpp
src/TimerController.cpp
src/UnMappedMemory.cpp
include/ALUHelpers.h
include/Address.h
include/BGMap.h
include/CartridgeHeader.h
include/Cpu.h
include/GameBoy.h
include/GlobalDefinitions.h
include/GraphicsManager.h
include/IEventHandler.h
include/IGraphicsHandler.h
include/IMemoryMappable.h
include/IODevices.h
include/InterruptController.h
include/JoypadController.h
include/MBC.h
include/Memory.h
include/PixelProcessingUnit.h
include/RAM.h
include/ROM.h
include/RegisterU16.h
include/RegisterU8.h
include/SerialTransferController.h
include/SoundController.h
include/Sprite.h
include/Tile.h
include/TimerController.h
include/UnMappedMemory.h
)

set_target_properties(GBLib PROPERTIES PUBLIC_HEADER
include/GameBoy.h
include/IGraphicsHandler.h
include/IEventHandler
)

target_include_directories(GBLib PRIVATE include)
target_include_directories(GBLib PRIVATE src)

add_compile_definitions(GAMEBOY_EXPORTING)