#pragma once
#include "GlobalDefinitions.h"
#include "CartridgeHeader.h"
#include "RAM.h"
#include "ROM.h"

enum MBC_RAM_SIZES
{
    NO_RAM,
    SMALL_RAM,
    ONE_BANK_RAM,
    FOUR_BANK_RAM
};

enum MBC1_BANK_MODE
{
    ROM_MODE,
    RAM_MODE
};

enum MBC3_REGISTER_MODE
{
    RTC_REG_MODE,
    RAM_REG_MODE
};

class MBC : public IMemoryMappable
{
public:
    virtual void LoadROMFromFile(std::string path) = 0;
    virtual void LoadRAMFromSave(std::string path) = 0;
    virtual void SaveToFile(std::string path) = 0;
};

class MBC1 : public MBC
{
private:
    CartridgeHeader& header;
    std::vector<ROM*> romBanks;
    std::vector<RAM*> ramBanks;
    int currentMode;

    u8 romBankRegister;
    u8 ramBankRegister;
    u8 extraBankRegister;

    bool ramEnabled;
    MBC_RAM_SIZES ramSize;

public: 
    MBC1(CartridgeHeader& header)
        : header(header), romBankRegister(1), ramBankRegister(0), currentMode(MBC1_BANK_MODE::ROM_MODE), ramEnabled(false)
    {
    }

    ~MBC1() {}

    void LoadROMFromFile(std::string path);
    void LoadRAMFromSave(std::string path) {}
    void Write(Address address, Byte value);
    Byte Read(Address address);
    u8 GetCurrentROMBank();
    u8 GetCurrentRAMBank();
    void SaveToFile(std::string path) {}
};

class MBC3 : public MBC
{
private:
    CartridgeHeader& header;
    std::vector<ROM*> romBanks;
    std::vector<RAM*> ramBanks;
    int currentMode;

    u8 romBankRegister;
    u8 ramBankRegister;
    u8 rtcRegister;
    u8 latchRegister;

    bool ramEnabled;
    MBC_RAM_SIZES ramSize;

public:
    MBC3(CartridgeHeader& header)
        : header(header), romBankRegister(0), ramBankRegister(0), rtcRegister(0), latchRegister(0xFF), currentMode(MBC3_REGISTER_MODE::RAM_REG_MODE), ramEnabled(false)
    {
    }

    ~MBC3() {}

    void LoadROMFromFile(std::string path);
    void LoadRAMFromSave(std::string path);
    void Write(Address address, Byte value);
    Byte Read(Address address);
    u8 GetCurrentROMBank();
    u8 GetCurrentRAMBank();
    void SaveToFile(std::string path);
};