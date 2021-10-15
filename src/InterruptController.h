#pragma once
#include "IMemoryMappable.h"
#include "RegisterU8.h"

enum INTERRUPT_FLAGS
{
    INT_JOYPAD = 16,
    INT_SERIAL = 8,
    INT_TIMER = 4,
    INT_LCD_STAT = 2,
    INT_VBLANK = 1
};

class InterruptController : public IMemoryMappable
{
private:
    Byte mem[2];
    RegisterU8 IE; // Interrupt Enable
    RegisterU8 IF; // Interrupt Flag

public:
    InterruptController();
    ~InterruptController() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);

    void RequestInterrupt(u8 flags);
    void ClearInterruptRequest(u8 flags);
    void EnableInterrupt(u8 flags);
    void DisableInterrupt(u8 flags);
    bool InterruptIsRequested(u8 flags);
    bool InterruptIsEnabled(u8 flags);
    void ClearAllRequests();
    void DisableAllInterrupts();
};