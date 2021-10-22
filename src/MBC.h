#pragma once
#include "GlobalDefinitions.h"
#include "CartridgeHeader.h"
#include "RAM.h"
#include "ROM.h"

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
    int currentROMBank;
    int currentRAMBank;
    int currentMode;
    bool ramEnabled;

public: 
    MBC1(CartridgeHeader& header)
        : header(header), currentROMBank(1), currentRAMBank(0), currentMode(0), ramEnabled(false)
    {
    }

    ~MBC1() {}

    void LoadFromFile(std::string& path);
    void Write(Address address, Byte value);
    Byte Read(Address address);
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