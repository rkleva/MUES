/*
 * board_setup.h
 *
 *  Created on: 14 Oct 2022
 *      Author: ZESA
 */

#include "F28x_Project.h"
#include "settings.h"
#include "DRV8301_SPI.h"
#include "drv8301_defines.h"
#include "dlog_4ch_f.h"

#ifndef BOARD_SETUP_H_
#define BOARD_SETUP_H_

#pragma INTERRUPT (MotorControlISR, HPI)


/******************************************************************************
Peripheral Assignments:
         - EPWMs    ==>> EPWM1, EPWM2,  EPWM3
         - Encoder  ==>> EQep1
         - Hall     ==>> EQep2
         - SPI      ==>> SpiA

         Analog signals:
         Vdc  ADC 14
         Va   ADC C3
         Vb   ADC B3
         Vc   ADC A3
         Ia   ADC C2
         Ib   ADC B2
         Ic   ADC A2

         Digital signals:
         nFAULT     GPIO19
         nOCTW      GPIO18
         EN_GATE    GPIO124
         DA_CAL     GPIO125

******************************************************************************/


// *************************************************
// ************ GPIO pin assignments ***************
// *************************************************

// *************************************************
// LED
// ========================
#define  BLUE_LED_GPIO          31
#define  BLUE_LED_MUX           0

#define  RED_LED_GPIO           34
#define  RED_LED_MUX            0


// *************************************************
// EPWM
// ========================
#define EPWM1A_PIN_GPIO         0
#define EPWM1A_PIN_MUX          1

#define EPWM1B_PIN_GPIO         1
#define EPWM1B_PIN_MUX          1

#define EPWM2A_PIN_GPIO         2
#define EPWM2A_PIN_MUX          1

#define EPWM2B_PIN_GPIO         3
#define EPWM2B_PIN_MUX          1

#define EPWM3A_PIN_GPIO         4
#define EPWM3A_PIN_MUX          1

#define EPWM3B_PIN_GPIO         5
#define EPWM3B_PIN_MUX          1


// *************************************************
// EQEP1 -> encoder signals
// ========================
#define EQEP1A_PIN_GPIO         20
#define EQEP1A_PIN_MUX          1

#define EQEP1B_PIN_GPIO         21
#define EQEP1B_PIN_MUX          1

#define EQEP1I_PIN_GPIO         99
#define EQEP1I_PIN_MUX          5


// *************************************************
// SPI-A -> communication with DRV8301
// =========================
#define  SPIA_MOSI_GPIO         58     /* Master Output Slave Input */
#define  SPIA_MOSI_MUX          15

#define  SPIA_MISO_GPIO         59      /* Master Input Slave Output */
#define  SPIA_MISO_MUX          15

#define  SPIA_CLK_GPIO          60      /* SPI Clock */
#define  SPIA_CLK_MUX           15

#define  SPIA_ENSL_GPIO      61      /* SPI Enable Slave */
#define  SPIA_ENSL_MUX       15




// *************************************************
// IO -> Hall sensor signals
// =========================
#define  HALL_A_PIN_GPIO        57
#define  HALL_A_PIN_MUX         0

#define  HALL_B_PIN_GPIO        55
#define  HALL_B_PIN_MUX         0

#define  HALL_C_PIN_GPIO        54
#define  HALL_C_PIN_MUX         0


// *************************************************
// IO -> signals to DRV8301
// =========================
#define  DRV_ENGATE_GPIO        124     /* DRV8301 - Enable gate and current shunt amplifiers */
#define  DRV_ENGATE_MUX         0

#define  DRV_DCCAL_GPIO         125     /* DRV8301 - Calibrate DC offset for current shunt amplifiers */
#define  DRV_DCCAL_MUX          0


// *************************************************
// IO -> signals from DRV8301
// =========================
#define  DRV_FAULT_GPIO         19      /* DRV8301 - Fault report indicator */
#define  DRV_FAULT_MUX          0

#define  DRV_OCTW_GPIO          18      /* DRV8301 - Overcurrent/overtemperature warning */
#define  DRV_OCTW_MUX           0




// *************************************************
// **************** Protections ********************
// *************************************************

#define  TRIP4   3

// *************************************************
// ********** Prototypes for functions *************
// *************************************************

void ConfigLedGpio(void);
void ConfigEPwmGpio(void);
void ConfigEQepGpio(void);
void ConfigSpiAGpio(void);
void ConfigHallGpio(void);
void ConfigDrvSignalsGpio(void);
void PwmTripConfig(volatile struct EPWM_REGS * PwmRegs, Uint16 TripNum);
void DMC_Protection(void);
void ConfigEPwmUpDwn(volatile struct EPWM_REGS * ePWM, Uint16 period);
void ConfigEPwmSlave(volatile struct EPWM_REGS * ePWM);
void ConfigEPwmSOC(volatile struct EPWM_REGS * ePWM);
void ConfigureADC(void);
void ADCChannelSetup(void);
void initialize_board(void);
interrupt void MotorControlISR(void);
void EQepRegsConfig(volatile struct EQEP_REGS * EQepRegs, Uint32 NPLS);

#endif /* BOARD_SETUP_H_ */
