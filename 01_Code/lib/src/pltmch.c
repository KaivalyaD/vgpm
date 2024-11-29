/* header files */
#include <SST89X5XXRD2.H>
#include "vgpmch.h"

/* emit an error on defining unsupported operations */
#ifdef _THREE_DIM_SUPPORT_
#line 13 "pltcfg.h"
#error A 3rd axis is not yet supported.
#endif	// _THREE_DIM_SUPPORT_

/*
// (time per real step) = ((time per virtual step) * 10 * r * pi) / (2 ^ n)
#define PM_PER_REAL_STEP_DELAY_MS ((float)((PM_DELAY_PER_STEP_MS) * (10.0f * PM_HALF_ROTATION_LINEAR_DISTANCE_MM)) / (float)(PM_STEP_DOWNSCALE)) */

/* global variables */
extern PMduration_ms pm_time_per_move_operation;

unsigned int pen_x = 0, pen_y = 0;
unsigned int steps_to_go_x, steps_to_go_y;

void pmInit(void)
{
	// code
	// disable both drivers
	N_EN0 = 1;
	N_EN1 = 1;
	
	// set mode to full-step
	MODE0_1 = 0;
	MODE1_1 = 0;
	MODE2_1 = 0;
	
	MODE0_2 = 0;
	MODE1_2 = 0;
	MODE2_2 = 0;
	
	// bring STEPX and DIRX to stable states
	DIR1 = OFF;
	STEP1 = OFF;
	
	DIR2 = OFF;
	STEP2 = OFF;
	
	// enable both drivers
	N_EN0 = 0;
	N_EN1 = 0;
}

void pmHome(void)
{
	// function prototypes
	void pmDelayms_T2(unsigned int);
	
	// code
	DIR1 = DIR_X_TOWARD_ORIGIN;
	DIR2 = DIR_Y_TOWARD_ORIGIN;
	
	// reset stepping pins
	STEP1 = OFF;
	STEP2 = OFF;
	
	// home
	while(!SW1 || !SW2)
	{
		if(!SW1)
		{
			STEP1 = ON;
			pmDelayms_T2(1);
			STEP1 = OFF;
		}
		if(!SW2)
		{
			STEP2 = ON;
			pmDelayms_T2(1);
			STEP2 = OFF;
		}
	}
}

void pmDelayms_T2(unsigned int ms)
{
	// code
	TR2 = OFF;
	TF2 = OFF;
	
	// configure Timer 2 as clock generator
	C_T2 = OFF;
	T2MOD = 0x10;
	while(ms-- > 0)
	{
		TH2 = 0xfc;
		TL2 = 0x18;
		TR2 = ON;
		
		while(TF2 == 0);
		
		TR2 = OFF;
		TF2 = OFF;
	}
}

PMresult movev(int *vtarget, int *vactual, unsigned int dim, unsigned char func, ...)
{
	// variable declarations
	PMduration_ms delay_per_x_step, delay_per_y_step;
	unsigned int delta_x, delta_y;

	// code
	if(dim > 3)
		return PM_FAILURE;
	
#ifndef _THREE_DIM_SUPPORT_
	if(dim > 2)
		return PM_FAILURE;
#endif
	
	// initial steady state
	DIR1 = OFF;
	DIR2 = OFF;
	STEP1 = OFF;
	STEP2 = OFF;
	
	switch(dim)
	{
	case 1:
		// (dim = 1) => x-motion only
		while(pen_x != *vtarget)
		{
			if(pen_x < PM_MIN_X || pen_x > PM_MAX_X)
				break;
			
			if(pen_x > *vtarget)
			{
				DIR1 = DIR_X_TOWARD_ORIGIN;
				
				STEP1 = ON;
				pmDelayms_T2(1);
				STEP1 = OFF;
				
				pen_x -= 1;
			}
			else
			{
				DIR1 = ~DIR_X_TOWARD_ORIGIN;
				
				STEP1 = ON;
				pmDelayms_T2(1);
				STEP1 = OFF;
				
				pen_x += 1;
			}
		}
		
		*vactual = pen_x;
		if(*vactual != *vtarget)
			return PM_FAILURE;
		break;
	
	case 2:
		// (dim = 2) => x- and y- simultaneous motion
		// setting upcoming heading and change
		if (*vtarget > pen_x)
		{
			DIR1 = ~DIR_X_TOWARD_ORIGIN;
			delta_x = *vtarget - pen_x;
		}
		else
		{
			DIR1 = DIR_X_TOWARD_ORIGIN;
			delta_x = pen_x - *vtarget;
		}

		if (*(vtarget + 1) > pen_y)
		{
			DIR2 = ~DIR_Y_TOWARD_ORIGIN;
			delta_y = *(vtarget + 1) - pen_y;
		}
		else
		{
			DIR2 = DIR_Y_TOWARD_ORIGIN;
			delta_y = pen_y - *(vtarget + 1);
		}

		// calculating the step count from deltas
		steps_to_go_x = (int)((float)(1000.0f * delta_x) / (float)PM_SINGLE_X_STEP_DISTANCE_UM);
		steps_to_go_y = (int)((float)(1000.0f * delta_y) / (float)PM_SINGLE_Y_STEP_DISTANCE_UM);

		// finding delays between each step of either motors
		delay_per_x_step = (PMduration_ms)((float)(pm_time_per_move_operation - (steps_to_go_x * PM_PULSE_WIDTH_MS)) / (float)(steps_to_go_x - 1));
		delay_per_y_step = (PMduration_ms)((float)(pm_time_per_move_operation - (steps_to_go_y * PM_PULSE_WIDTH_MS)) / (float)(steps_to_go_y - 1));

		// setting timers for actual delay generation


		break;
	}
	
	return PM_SUCCESS;
}

/* entry-point function */
int main(void)
{
	// local variables
	int x;
	int r = 0;
	
	// code
	pmInit();
	pmHome();
	
	x = 5;
	if(movev(&x, &r, 1, PM_FUNC_LINEAR) != PM_SUCCESS)
	{
		P1 = 0x01 << 7;
	}
	
	// task loop
	while(1)
	{
		// repeating code
	}
	
	return 0;
}
