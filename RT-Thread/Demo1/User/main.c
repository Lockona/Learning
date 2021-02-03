#include "ARMCM4.h"

uint32_t flag1 =0;
uint32_t flag2 =0;

struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;


void delay(uint32_t count)
{
	for(;count;count--);
}

void flag1_thread_entry(void *p_arg)
{
	for(;;)
	{
		flag1 = 1;
		delay(100);
		flag1 = 0;
		delay(100);
		
		rt_schedule();
	}
}

void flag2_thread_entry(void *p_arg)
{
	for(;;)
	{
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
		
		rt_schedule();
	}
}

int main()
{
	for(;;)
	{
		flag1 = 0;
	}
}

