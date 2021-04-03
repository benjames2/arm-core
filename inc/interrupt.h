#pragma once
#include <inc/armstate.h>
#include <inc/stack_operations.h>

struct special_register_t {

    const static unsigned int EXTINT       = 0x400FC140;
    const static unsigned int EXTMODE      = 0x400FC148;
    const static unsigned int EXTPOLAR     = 0x400FC14C;
    const static unsigned int PINSEL4      = 0x4002C010;

    const static unsigned int RICOMPVAL    = 0x400B0000;
    const static unsigned int RIMASK       = 0x400B0004;
    const static unsigned int RICTRL       = 0x400B0008;
    const static unsigned int RICOUNTER    = 0x400B000C;
    const static unsigned int NVIC_ENABLE0 = 0xE000E100;
};

enum interrupt_id {
    WDT,      TIMER0,
    TIMER1,   TIMER2,
    TIMER3,   UART0,
    UART1,    UART2,
    UART3,    PWM1,
    I2C0,     I2C1,
    I2C2,     SPI,
    SPP0,     SSP1,   
    PLL0,     RTC,
    EINT0,    EINT1,
    EINT2,    EINT3,
    ADC,      BOD,
    USB,      CAN,
    GPDMA,    I2S,
    ETHERNET, RIT
};

uint32_t get_exception_number(interrupt_id id);
static void enable_rit(armstate_t& armstate);
static bool rit_enabled();
void rit_procedure(armstate_t& armstate);
bool rit_triggered(armstate_t& armstate);
armstate_t rit_handler(armstate_t& armstate);

static bool eint0_enabled(armstate_t& armstate);

armstate_t call_isr(armstate_t& armstate_t, interrupt_id id);