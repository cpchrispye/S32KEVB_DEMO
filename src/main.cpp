#include "scheduler.h"




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

uint32_t stack_a[0x100];
uint32_t stack_b[0x100];
static int abc = 0;

bool low_event(void)
{
    return abc < 50;
}
bool high_event(void)
{
    return abc >= 50;
}

void task_a(void)
{


	volatile float c = 1.2;
	while(1)
	{
        sch_sleep(5 * 1000 * 1000);
        pin::set(pin::PORT::D, 0, true);
        abc = 100;
	}

}

void task_b(void)
{
	volatile int c = 1000;
	while(1)
	{
        sch_event(high_event);
        pin::set(pin::PORT::D, 0, false);
        abc = 33;
        
	}
}

int main(void)
{
    sch_add_task( task_a, false, (uint8_t *)stack_a, sizeof( stack_a ) );
    sch_add_task( task_b, true, (uint8_t *)stack_b, sizeof( stack_b ) );
	
	volatile int c = 1;
	//testa();
    //SystemInit();
    
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
	SysTick_Config(SystemCoreClock / 100); // tick 10000 ticks per second

    pin::init(pin::PORT::D, 0, pin::MUX::GPIO, false);

    sch_run_tasks( 10000 );
	while(1)
	{
        ;
	}
}
