#pragma once
#include "GlobalDefinitions.h"
#include "CartridgeHeader.h"
#include "RAM.h"
#include "ROM.h"

enum MBC1_RAM_SIZES
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

class MBC : public IMemoryMappable
{
public:
    virtual void LoadFromFile(std::string& path) = 0;
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
    MBC1_RAM_SIZES ramSize;

public: 
    MBC1(CartridgeHeader& header)
        : header(header), romBankRegister(1), ramBankRegister(0), currentMode(MBC1_BANK_MODE::ROM_MODE), ramEnabled(false)
    {
    }

    ~MBC1() {}

    void LoadFromFile(std::string& path);
    void Write(Address address, Byte value);
    Byte Read(Address address);
    u8 GetCurrentROMBank();
    u8 GetCurrentRAMBank();
};

class MBC3 : public MBC
{
public:
    MBC3(CartridgeHeader& header) {}
    ~MBC3() {}

    void LoadFromFile(std::string& path);
    void Write(Address address, Byte value);
    Byte Read(Address address);
};