#include "TimerController.h"

void TimerController::Write(Address address, Byte value)
{
    if (address == ADDR_TIMER_REG_DIV)
    {
        this->DIV = 0;
    }
    else
    {
        mem[address - ADDR_TIMER_START] = value;
    }
}

Byte TimerController::Read(Address address)
{
    return mem[address - ADDR_TIMER_START];
}

void TimerController::Tick(u64 cycles)
{
    this->clockCyclesDiv += cycles;

    // Div clock always increments
    if (this->clockCyclesDiv >= CLOCKS_PER_DIVIDER_INC)
    {
        ++DIV;
        this->clockCyclesDiv -= CLOCKS_PER_DIVIDER_INC;
    }

    if (TimerEnabled())
    {
        this->clockCyclesTimer += cycles;

        if (this->clockCyclesTimer >= GetTimerFrequencyClocks())
        {
            // Overflow, fire an interrupt
            if (this->TIMA == 0xFF)
            {
                this->TIMA = this->TMA;
                this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_TIMER);
            }
            else
            {
                ++TIMA;
            }

            this->clockCyclesTimer -= GetTimerFrequencyClocks();
        }
    }
}

bool TimerController::TimerEnabled()
{
    return this->TAC.FlagIsSet(TIMER_FLAGS::TIMER_ENABLED);
}

int TimerController::GetTimerFrequencyClocks()
{
    int mask = 0b00000011;
    int bits = this->TAC & mask;

    switch (bits)
    {
    case 0:
        return 256;
        break;
    case 1:
        return 4;
        break;
    case 2:
        return 16;
        break;
    case 3:
        return 64;
        break;
    default:
        throw std::runtime_error("Invalid Timer Frequency.");
    }
}