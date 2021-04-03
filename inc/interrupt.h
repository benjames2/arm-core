#include <inc/armstate.h>

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



static void rit_enable(armstate_t& armstate);
void rit_procedure(armstate_t& armstate);
bool rit_triggered(armstate_t& armstate);
armstate_t rit_handler(armstate_t& armstate);

static bool eint0_enabled(armstate_t& armstate);

armstate_t call_isr(armstate_t& armstate_t);