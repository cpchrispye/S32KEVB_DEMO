#include "scheduler.h"
#include "safe_var.h"



#include "device_registers.h"
#include "system_S32K146.h"

#include "driver_pins.h"

using namespace driver;

__asm(".global __use_no_semihosting\n\t");

extern "C"
{
void _sys_exit(int ch) { }
void _sys_command_string(int ch) { }
void _ttywrch(int ch) { }
}

static volatile float st = 0.0;
safe_var<float> sv(1.2);

uint32_t stack_a[0x100];
uint32_t stack_b[0x100];
static int abc = 0;

bool low_event(void)
{
    ASSERT(sv.test());
    return sv < 50;
}
bool high_event(void)
{
    if(!sv.test())
    {
        while(1)
            ;
    }
    return sv >= 50.0;
}

void task_a(void)
{


	volatile float c = 1.2;
	while(1)
	{
        sch_sleep(5 * 1000 * 1000);
        pin::set(pin::PORT::D, 0, true);
        sv = 100;
	}

}

void task_b(void)
{
	volatile int c = 1000;
	while(1)
	{
        sch_event(high_event);
        pin::set(pin::PORT::D, 0, false);
        sv = 33;
        
	}
}

int main(void)
{
    safe_var<int> si(10);
    safe_var<float> sv(1.2);
    
    sch_add_task( task_a, false, (uint8_t *)stack_a, sizeof( stack_a ) );
    sch_add_task( task_b, true, (uint8_t *)stack_b, sizeof( stack_b ) );
	
	volatile int c = 1;
	//testa();
    //SystemInit();
    
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
	SysTick_Config(SystemCoreClock / 100); // tick 10000 ticks per second

    pin::init(pin::PORT::D, 0, pin::MUX::GPIO, true, pin::PULL::NONE);
    
    si += 10;
    sv += 10;
    sch_run_tasks( 10000 );
	while(1)
	{
        ;
	}
}
