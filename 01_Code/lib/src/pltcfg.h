#ifndef _PLTCFG_H
#define _PLTCFG_H

/* direction */
#define DIR_X_TOWARD_ORIGIN 0
#define DIR_Y_TOWARD_ORIGIN 0

/* limits */
#define PM_MIN_X 0
#define PM_MAX_X 10000

/* switches */
// #define _THREE_DIM_SUPPORT_	// currently unsupported

/* hardware abstraction */
sbit N_EN0 = P1^0;
sbit MODE0_1 = P1^1;
sbit MODE1_1 = P1^2;
sbit MODE2_1 = P1^3;
sbit STEP1 = P1^4;
sbit DIR1 = P1^5;
sbit SW1 = P3^0;

sbit N_EN1 = P1^1;
sbit MODE0_2 = P2^2;
sbit MODE1_2 = P2^3;
sbit MODE2_2 = P2^4;
sbit STEP2 = P2^1;
sbit DIR2 = P2^0;
sbit SW2 = P3^1;

/* reference time */
#define PM_PULSE_WIDTH_MS 1

/* reference distances */
#define PM_SINGLE_X_STEP_DISTANCE_UM 100.0f	// 0.1 mm
#define PM_SINGLE_Y_STEP_DISTANCE_UM 100.0f	// 0.1 mm

#endif	// _PLTCFG_H
