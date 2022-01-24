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

enum MBC_BANK_MODE
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
protected:
    CartridgeHeader& header;
    std::vector<ROM*> romBanks;
    std::vector<RAM*> ramBanks;
    int currentMode;
    bool ramEnabled;
    MBC_RAM_SIZES ramSize;

public:
    virtual void LoadROMFromFile(std::string path);
    virtual void LoadRAMFromSave(std::string path);
    virtual void SaveToFile(std::string path);

    MBC(CartridgeHeader& header, int currentMode, bool ramEnabled)
        : header(header), romBanks(), ramBanks(), currentMode(currentMode), ramEnabled(ramEnabled), ramSize(MBC_RAM_SIZES::NO_RAM)
    {
    }

    ~MBC()
    {
        for (ROM* obj : this->romBanks)
        {
            delete obj;
        }

        for (RAM* obj : this->ramBanks)
        {
            delete obj;
        }
    }
};

class MBC1 : public MBC
{
private:
    u8 romBankRegister;
    u8 ramBankRegister;
    u8 extraBankRegister;

    MBC_RAM_SIZES ramSize;

public: 
    MBC1(CartridgeHeader& header)
        :   MBC(header, MBC_BANK_MODE::ROM_MODE, false), 
            romBankRegister(1), ramBankRegister(0), extraBankRegister(0)
    {
    }

    ~MBC1() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
    u8 GetCurrentROMBank();
    u8 GetCurrentRAMBank();
};

class MBC2 : public MBC
{
private:
    u8 romBankRegister;

public:
    MBC2(CartridgeHeader& header)
        : MBC(header, MBC_BANK_MODE::ROM_MODE, false),
        romBankRegister(1)
    {
        RAM* ram = new RAM(ADDR_MBC2_RAM_START, ADDR_MBC2_RAM_END);
        this->ramBanks.push_back(ram);
    }

    ~MBC2() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
};

class MBC3 : public MBC
{
private:
    u8 romBankRegister;
    u8 ramBankRegister;
    u8 currentRtcRegister;
    u8 rtcRegisters[5] = {0};
    u8 latchRegister;

public:
    MBC3(CartridgeHeader& header)
        :   MBC(header, MBC3_REGISTER_MODE::RAM_REG_MODE, false),
            romBankRegister(0), ramBankRegister(0), currentRtcRegister(0), latchRegister(0xFF)
    {
    }

    ~MBC3() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
    u8 GetCurrentROMBank();
    u8 GetCurrentRAMBank();
};