//=============================================================================
/*!
    \file       driver_pins.cpp

    \brief      driver_pins

                Template text

    \author     chris.pye


 */
//==============================================================================

/*
===============================================================================
    INCLUDES
===============================================================================
*/

#include "driver_pins.h"
#include "device_registers.h"
#include "scheduler.h"


/*
===============================================================================
    CONSTANTS
===============================================================================
*/
PORT_Type * const PORT_REGS[] = PORT_BASE_PTRS;
GPIO_Type * const GPIO_REGS[] = GPIO_BASE_PTRS;
/*
===============================================================================
DATA STRUCTURES AND TYPEDEFS
===============================================================================
*/

/*
===============================================================================
    VARIABLES
===============================================================================
*/

/*
===============================================================================
    PROTOTYPES FOR INTERNAL MODULE FUNCTIONS
===============================================================================
*/

/*
===============================================================================
    CLASSES
===============================================================================
*/

/*
===============================================================================
    FUNCTIONS INTERNAL TO THE MODULE
===============================================================================
*/
namespace driver {
namespace pin {
//========================================================================
/*!
    function    function_name()

    \brief      Template text
	
	\param		Template text

    \return     Template text
*/
//========================================================================

void init( PORT port, uint32_t pin, MUX mux, bool output, PULL pull )
{
    auto port_i = static_cast<uint32_t>(port);
    auto mux_i  = static_cast<uint32_t>(mux);

    ASSERT(pin  >= PORT_PCR_COUNT);
    ASSERT(port_i >= static_cast<uint32_t>(PORT::E));
    ASSERT(mux_i  >= static_cast<uint32_t>(MUX::ALT_7));
    ASSERT(pull_i >= static_cast<uint32_t>(PULL::DOWN)); // comparing values as uint so that we dont have to test that enums are positive aswell

    PCC->PCCn[PCC_PORTA_INDEX + port_i] |= PCC_PCCn_CGC( true ); // enable port clock

    auto &port_obj = PORT_REGS[port_i];
    auto &gpio_obj = GPIO_REGS[port_i];

    uint32_t tmp = gpio_obj->PDDR & ~(1U << pin);
    gpio_obj->PDDR = tmp | static_cast<uint32_t >(output << pin);

    port_obj->PCR[pin] = PORT_PCR_MUX( mux_i )
                         | PORT_PCR_PE( pull != PULL::NONE )
                         | PORT_PCR_PS( pull == PULL::UP )
                         | PORT_PCR_PFE( 0 );
}


void set( PORT port, uint32_t pin, bool input)
{
    auto port_i = static_cast<uint32_t>(port);

    ASSERT(pin  >= PORT_PCR_COUNT);
    ASSERT(port_i >= static_cast<uint32_t>(PORT::E));
    auto &gpio_obj = GPIO_REGS[port_i];
    gpio_obj->PDOR = (gpio_obj->PDOR & static_cast<uint32_t >(~(1U << pin)))
                   | static_cast<uint32_t >(input << pin);
}
void set( PORT port, uint32_t bits)
{
    auto port_i = static_cast<uint32_t>(port);
    ASSERT(port_i >= static_cast<uint32_t>(PORT::E));

    auto &gpio_obj = GPIO_REGS[port_i];
    gpio_obj->PDOR = bits;
}

bool get( PORT port, uint32_t pin)
{
    auto port_i = static_cast<uint32_t>(port);
    ASSERT(port_i >= static_cast<uint32_t>(PORT::E));
    return (GPIO_REGS[port_i]->PDIR & (1U << pin)) != 0U;
}

uint32_t get( PORT port)
{
    auto port_i = static_cast<uint32_t>(port);
    ASSERT(port_i >= static_cast<uint32_t>(PORT::E));
    return GPIO_REGS[port_i]->PDIR;
}

}// namespace pin

} // namespace driver

/*
===============================================================================
*/