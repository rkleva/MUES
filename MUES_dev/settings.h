/*
 * settings.h
 *
 *  Created on: 14 Oct 2022
 *      Author: Luka Pravica
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#ifdef _LAUNCHXL_F28379D
    #include "F28x_Project.h"
    #include "math.h"
#else
    #include "tmwtypes.h"
    typedef int                                     int16;
    typedef long                                    int32;
    typedef long long                               int64;
    typedef unsigned int                            Uint16;
    typedef unsigned long                           Uint32;
    typedef unsigned long long                      Uint64;
    typedef float                                   float32;
    typedef long double                             float64;
#endif



#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

/* constants */
#define PI               3.14159265359f
#define TWO_PI           6.28318530718f
#define SQRT3            1.73205080757f
#define SQRT3_OVER_TWO   0.86602540378f
#define ONE_OVER_PI      0.31830988618f
#define ONE_OVER_SQRT3   0.57735026919f
#define TWO_OVER_SQRT3   1.15470053838f
#define ONE_OVER_THREE   0.33333333333f

/* Define the system frequency (MHz) */
#define SYSTEM_FREQUENCY 200

/* Define the ISR frequency (kHz) */
#ifdef _LAUNCHXL_F28379D
#define  PWM_FREQUENCY        10
#define  ISR_FREQUENCY        PWM_FREQUENCY
#define  CTRL_TS              0.001/PWM_FREQUENCY
#define  INV_PWM_TICKS        ((SYSTEM_FREQUENCY)/PWM_FREQUENCY)*1000/2
#define  INV_PWM_TBPRD        INV_PWM_TICKS/2
#define  INV_PWM_HALF_TBPRD   INV_PWM_TICKS/4
#else
    #define  CTRL_TS              0.001f      /*   1/PWM_FREQUENCY/1000    */
    #define  INV_PWM_TBPRD        5000
#endif


//options for BASE_CURRENT based on DRV8301 current-sense amplifier gain setting
#define DRV_GAIN  10
//#define DRV_GAIN  20
//#define DRV_GAIN  40
//#define DRV_GAIN  80



#ifndef _LAUNCHXL_F28379D
    void MotorControlISR(uint16_T MatlabAdcxRegs[6], real_T* MatlabValuesSI, real_T matlab_enable, real_T matlab_ref, uint16_T* CMP_PWM);
#endif
    
#endif /* SETTINGS_H_ */
