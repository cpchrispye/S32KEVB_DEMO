//
// Created by chris.pye on 22/06/2020.
//

#ifndef S32KEVB_DEMO_SAFE_VAR_H
#define S32KEVB_DEMO_SAFE_VAR_H
//=============================================================================
/*!
    \file       safe_var.h

    \brief      safe_var

                Template text

    \author     chris.pye

*/
//=============================================================================
#include <type_traits>


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
========================================================================
    INTERFACE FUNCTIONS
========================================================================
*/

/*
========================================================================
    CLASS
========================================================================
*/
    template <typename T, typename enable = void>
    class safe_var {
        //static_assert( false, "type unsupported\n" );
    };

    template<typename T>
    class safe_var<T, typename std::enable_if<std::is_integral<T>::value>::type>{

    public:
        explicit safe_var(T value = 0): value( value), inverse( ~value){}
        T get() const { return value;}
        void set(T val){ 
            __asm("CPSID I\n");
            value = val; 
            inverse = ~val;
            __asm("CPSIE I\n");
        }
        bool test() const { return value == static_cast<T>(~inverse);}
        operator T() const {return value;}
        safe_var& operator=(T val) { set(val); return *this;}
        safe_var& operator++() {set(++value); return *this;}
        safe_var& operator--() {set(--value); return *this;}
        safe_var& operator+=(T val) {set(value += val); return *this;}
        safe_var& operator-=(T val) {set(value -= val); return *this;}
        safe_var& operator*=(T val) {set(value *= val); return *this;}
        safe_var& operator/=(T val) {set(value /= val); return *this;}

    private:
        T value;
        volatile T inverse;
    };

    template<typename T>
    class safe_var <T, typename std::enable_if<std::is_floating_point<T>::value>::type>{

    public:
        explicit safe_var(T value = 0){set(value);}
        T get() const { return data.value;}
        void set(T val){
            __asm("CPSID I\n");
            data.value = val;
            for(int i = 0; i < sizeof(T); i++ )
            {
                inverse[i] =~(data.bytes[i]);
            }
            __asm("CPSIE I\n");
        }
        bool test() const {
            __asm("CPSID I\n");
            for(int i = 0; i < sizeof(T); i++ )
            {
                if(inverse[i] != static_cast<unsigned char>(~(data.bytes[i])))
                {
                    __asm("CPSIE I\n");
                    return false;
                }
            }
            __asm("CPSIE I\n");
            return true;
        }
        operator T() const {return data.value;}
        safe_var& operator=(T val) { set(val); return *this;}
        safe_var& operator++() {set(++data.value); return *this;}
        safe_var& operator--() {set(--data.value); return *this;}
        safe_var& operator+=(T val) {set(data.value += val); return *this;}
        safe_var& operator-=(T val) {set(data.value -= val); return *this;}
        safe_var& operator*=(T val) {set(data.value *= val); return *this;}
        safe_var& operator/=(T val) {set(data.value /= val); return *this;}

    private:
        union view {
            T value;
            unsigned char bytes[sizeof(T)];
        } data ;            
        volatile unsigned char inverse[sizeof(T)];
    };


/*
===============================================================================
    END OF FILE
===============================================================================
*/
#endif //S32KEVB_DEMO_SAFE_VAR_H
