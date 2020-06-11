#pragma once
//=============================================================================
/*!
    \file       driver_pins.h

    \brief      driver_pins

                Template text

    \author     chris.pye

*/
//=============================================================================

/*
===============================================================================
    INCLUDES
===============================================================================
*/
#include <cstdint>


namespace driver {
    namespace pin {
/*
=========================================================================
DATA STRUCTURES AND TYPE DEFINITIONS
=========================================================================
*/

/*
========================================================================
    EXTERNAL VARIABLES
========================================================================
*/

/*
===============================================================================
    CLASSES
===============================================================================
*/
        enum class PORT {
            A,
            B,
            C,
            D,
            E,
        };

        enum class MUX {
            DISABLED,
            GPIO,
            ALT_2,
            ALT_3,
            ALT_4,
            ALT_5,
            ALT_6,
            ALT_7,
        };

        enum class PULL {
            NONE,
            UP,
            DOWN,
        };

/*
========================================================================
    INTERFACE FUNCTIONS
========================================================================
*/

        void init( PORT port, uint32_t pin, MUX mux, bool input, PULL pull = PULL::NONE );
        void set( PORT port, uint32_t pin, bool input);
        void set( PORT port, uint32_t bits);
        bool get( PORT port, uint32_t pin);
        uint32_t get( PORT port);

    } // namespace pin
} // namespace driver
/*
===============================================================================
    END OF FILE
===============================================================================
*/

