/*
 * board_setup.c
 *
 *  Created on: 14 Oct 2022
 *      Author: ZESA
 */


#include "board_setup.h"

DRV8301_Vars drv_data = DRV8301_DEFAULTS;

/**
 * \brief           Initialize All Systems, GPIO and functions used on board
 */
void initialize_board(void){


/* PLL, WatchDog, enable Peripheral Clocks */
    InitSysCtrl();

/* Initialize GPIO: */
    InitGpio();

/* Disable CPU interrupts */
    DINT;

/* Initialize the PIE control registers to their default state. */
    InitPieCtrl();

/* Disable CPU interrupts and clear all CPU interrupt flags: */
    IER = 0x0000;
    IFR = 0x0000;

/* Initialize the PIE vector table with pointers to the shell Interrupt
   Service Routines (ISR). */
    InitPieVectTable();

/* Configure GPIO-s for LED signalization */
    ConfigLedGpio();

/* Initialize EPWM modules for inverter PWM generation */
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;
    ConfigEPwmUpDwn(&EPwm1Regs, INV_PWM_TICKS);
    ConfigEPwmUpDwn(&EPwm2Regs, INV_PWM_TICKS);
    ConfigEPwmUpDwn(&EPwm3Regs, INV_PWM_TICKS);

    ConfigEPwmSlave(&EPwm2Regs);
    ConfigEPwmSlave(&EPwm3Regs);

    ConfigEPwmSOC(&EPwm1Regs);

    ConfigEPwmGpio();

/* Initialize EPWM modules for inverter PWM generation */
    ConfigureADC();
    ADCChannelSetup();

    EALLOW;
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL  = 2; /* ADC B EOC of SOC2 is used to trigger Motor control Interrupt */
    AdcbRegs.ADCINTSEL1N2.bit.INT1CONT = 1;
    AdcbRegs.ADCINTSEL1N2.bit.INT1E    = 1;

    PieVectTable.ADCB1_INT         = &MotorControlISR;
    PieCtrlRegs.PIEIER1.bit.INTx2  = 1;  /* Enable ADCB1INT in PIE group 1 */

    IER |= M_INT1;                       /* Enable group 1 interrupts */

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;              // Start all the timers synced

    EDIS;

/* Initialize QEP module */
    ConfigEQepGpio();

    EQepRegsConfig(&EQep1Regs, 4000);

/* Initialize SPI module for communication with DRV8301 */
    ConfigSpiAGpio();
    ConfigDrvSignalsGpio();

    DRV8301SPIInit(&SpiaRegs);
    InitDrvData(&drv_data);
    GPIO_WritePin(DRV_ENGATE_GPIO, 1);  // Enable DRV8301
    DELAY_US(50000);                    // delay to allow DRV8301 supplies to ramp up

    InitDRV8301(&SpiaRegs,&drv_data,SPIA_CLK_GPIO,SPIA_ENSL_GPIO);
    while (drv_data.DRV_fault) ;  // hang on if drv init is faulty

    ConfigHallGpio();


    DMC_Protection();
    PwmTripConfig(&EPwm1Regs, TRIP4);
    PwmTripConfig(&EPwm2Regs, TRIP4);
    PwmTripConfig(&EPwm3Regs, TRIP4);


    GPIO_WritePin(DRV_DCCAL_GPIO,  0);      // Set DC_CAL to 0 to enable current amplifiers
    DELAY_US(5);                            // delay to allow DRV8301 amplifiers to settle

/* Enable Interrupts */
    EALLOW;
    EINT;          // Enable Global interrupt INTM
    ERTM;          // Enable Global realtime interrupt DBGM
    EDIS;

}





/**
 * \brief           LED GPIO Configuration
 */
void ConfigLedGpio(void)
{
    // set up Blue LED
    GPIO_SetupPinOptions(BLUE_LED_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(BLUE_LED_GPIO, GPIO_MUX_CPU1, BLUE_LED_MUX);
    GPIO_WritePin(BLUE_LED_GPIO, 1);

    // set up Red LED
    GPIO_SetupPinOptions(RED_LED_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(RED_LED_GPIO, GPIO_MUX_CPU1, RED_LED_MUX);
    GPIO_WritePin(RED_LED_GPIO, 1);

    return;
}


/**
 * \brief           EPWM GPIO Configuration
 */
void ConfigEPwmGpio(void)
{
    // set up EPWM1-A
    GPIO_SetupPinOptions(EPWM1A_PIN_GPIO, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinMux(EPWM1A_PIN_GPIO, GPIO_MUX_CPU1, EPWM1A_PIN_MUX);

    // set up EPWM1-B
    GPIO_SetupPinOptions(EPWM1B_PIN_GPIO, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinMux(EPWM1B_PIN_GPIO, GPIO_MUX_CPU1, EPWM1B_PIN_MUX);

    // set up EPWM2-A
    GPIO_SetupPinOptions(EPWM2A_PIN_GPIO, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinMux(EPWM2A_PIN_GPIO, GPIO_MUX_CPU1, EPWM2A_PIN_MUX);

    // set up EPWM2-B
    GPIO_SetupPinOptions(EPWM2B_PIN_GPIO, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinMux(EPWM2B_PIN_GPIO, GPIO_MUX_CPU1, EPWM2B_PIN_MUX);

    // set up EPWM3-A
    GPIO_SetupPinOptions(EPWM3A_PIN_GPIO, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinMux(EPWM3A_PIN_GPIO, GPIO_MUX_CPU1, EPWM3A_PIN_MUX);

    // set up EPWM3-B
    GPIO_SetupPinOptions(EPWM3B_PIN_GPIO, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinMux(EPWM3B_PIN_GPIO, GPIO_MUX_CPU1, EPWM3B_PIN_MUX);

    return;
}

/**
 * \brief           EQEP1 GPIO Configuration
 */
void ConfigEQepGpio(void)
{
    // set up EQEP1-A
    GPIO_SetupPinOptions(EQEP1A_PIN_GPIO, GPIO_INPUT, GPIO_SYNC);
    GPIO_SetupPinMux(EQEP1A_PIN_GPIO, GPIO_MUX_CPU1, EQEP1A_PIN_MUX);

    // set up EQEP1-B
    GPIO_SetupPinOptions(EQEP1B_PIN_GPIO, GPIO_INPUT, GPIO_SYNC);
    GPIO_SetupPinMux(EQEP1B_PIN_GPIO, GPIO_MUX_CPU1, EQEP1B_PIN_MUX);

    // set up EQEP1-I
    GPIO_SetupPinOptions(EQEP1B_PIN_GPIO, GPIO_INPUT, GPIO_SYNC);
    GPIO_SetupPinMux(EQEP1I_PIN_GPIO, GPIO_MUX_CPU1, EQEP1I_PIN_MUX);

    return;
}


/**
 * \brief           SPI-A GPIO Configuration
 */
void ConfigSpiAGpio(void)
{
    // set up SPI-A MOSI
    GPIO_SetupPinOptions(SPIA_MOSI_GPIO, GPIO_INPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(SPIA_MOSI_GPIO, GPIO_MUX_CPU1, SPIA_MOSI_MUX);

    // set up SPI-A MISO
    GPIO_SetupPinOptions(SPIA_MISO_GPIO, GPIO_INPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(SPIA_MISO_GPIO, GPIO_MUX_CPU1, SPIA_MISO_MUX);

    // set up SPI-A Clock
    GPIO_SetupPinOptions(SPIA_CLK_GPIO, GPIO_INPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(SPIA_CLK_GPIO, GPIO_MUX_CPU1, SPIA_CLK_MUX);

    // set up SPI-A Enable slave
    GPIO_SetupPinOptions(SPIA_ENSL_GPIO, GPIO_INPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(SPIA_ENSL_GPIO, GPIO_MUX_CPU1, SPIA_ENSL_MUX);

    return;
}

/**
 * \brief           Hall GPIO Configuration
 */
void ConfigHallGpio(void)
{
    // set up Hall A
    GPIO_SetupPinOptions(HALL_A_PIN_GPIO, GPIO_INPUT, GPIO_SYNC);
    GPIO_SetupPinMux(HALL_A_PIN_GPIO, GPIO_MUX_CPU1, HALL_A_PIN_MUX);

    // set up Hall B
    GPIO_SetupPinOptions(HALL_B_PIN_GPIO, GPIO_INPUT, GPIO_SYNC);
    GPIO_SetupPinMux(HALL_B_PIN_GPIO, GPIO_MUX_CPU1, HALL_B_PIN_MUX);

    // set up Hall C
    GPIO_SetupPinOptions(HALL_C_PIN_GPIO, GPIO_INPUT, GPIO_SYNC);
    GPIO_SetupPinMux(HALL_C_PIN_GPIO, GPIO_MUX_CPU1, HALL_C_PIN_MUX);

    return;
}


/**
 * \brief           Drive IO GPIO Configuration
 */
void ConfigDrvSignalsGpio(void)
{
    // set up Drive IO: Enable gate
    GPIO_WritePin(DRV_ENGATE_GPIO, 0); // Disable EN_GATE
    GPIO_SetupPinOptions(DRV_ENGATE_GPIO, GPIO_OUTPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(DRV_ENGATE_GPIO, GPIO_MUX_CPU1, DRV_ENGATE_MUX);

    // set up Drive IO: DC offset calibration
    GPIO_WritePin(DRV_DCCAL_GPIO, 1);  // Set DC_CAL to 1 to disable amplifiers
    GPIO_SetupPinOptions(DRV_DCCAL_GPIO, GPIO_OUTPUT, GPIO_ASYNC);
    GPIO_SetupPinMux(DRV_DCCAL_GPIO, GPIO_MUX_CPU1, DRV_DCCAL_MUX);

    // set up Drive IO: OCTW - (active low), trip PWM based on this
    GPIO_SetupPinOptions(DRV_FAULT_GPIO, GPIO_INPUT, GPIO_INVERT);
    GPIO_SetupPinMux(DRV_FAULT_GPIO, GPIO_MUX_CPU1, DRV_FAULT_MUX);

    // set up Drive IO: OCTW - (active low), trip PWM based on this
    GPIO_SetupPinOptions(DRV_OCTW_GPIO, GPIO_INPUT, GPIO_INVERT);
    GPIO_SetupPinMux(DRV_OCTW_GPIO, GPIO_MUX_CPU1, DRV_OCTW_MUX);

    return;
}


/**
 * \brief           Configurate PWM for trip action
 */
void PwmTripConfig(volatile struct EPWM_REGS * PwmRegs, Uint16 TripNum)
{
    EALLOW;

    PwmRegs->DCTRIPSEL.bit.DCAHCOMPSEL = TripNum; //TripNum is input to DCAHCOMPSEL
    PwmRegs->TZDCSEL.bit.DCAEVT1       = TZ_DCAH_HI;
    PwmRegs->DCACTL.bit.EVT1SRCSEL     = DC_EVT1;
    PwmRegs->DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;
    PwmRegs->TZSEL.bit.DCAEVT1         = 1;
    PwmRegs->TZSEL.bit.CBC6    = 0x1;         // Emulator Stop
    PwmRegs->TZCTL.bit.TZA     = TZ_FORCE_LO; // TZA event force EPWMxA go low
    PwmRegs->TZCTL.bit.TZB     = TZ_FORCE_LO; // TZB event force EPWMxB go low
    PwmRegs->TZCLR.bit.DCAEVT1 = 1;           // Clear any spurious OV trip
    PwmRegs->TZCLR.bit.OST     = 1;           // clear any spurious OST set early

    EDIS;
}


/**
 * \brief           DMC Protection Against Over Current
 */
void DMC_Protection(void)
{


    EALLOW;

    // Enable Mux 0  OR Mux 4 to generate TRIP4
    // Clear everything first
    EPwmXbarRegs.TRIP4MUX0TO15CFG.all  = 0x0000;    // clear all MUXes - MUX  0 to 15
    EPwmXbarRegs.TRIP4MUX16TO31CFG.all = 0x0000;    // clear all MUXes - MUX 16 to 31
    EPwmXbarRegs.TRIP4MUXENABLE.all    = 0x0000;    // Disable all the muxes first

    // Enable Muxes for ORed input of InputXbar1, InputXbar2
    InputXbarRegs.INPUT1SELECT = DRV_FAULT_GPIO; // Select FAULT as INPUTXBAR1
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX1 = 1;     // Enable Mux for ored input of inputxbar1
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX1   = 1;     // Enable MUX1 to generate TRIP4

    InputXbarRegs.INPUT2SELECT = DRV_OCTW_GPIO;  // Select OCTW as INPUTXBAR2
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX3 = 1;     // Enable Mux for ored input of inputxbar2
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX3   = 1;     // Enable MUX3 to generate TRIP4

    EDIS;

    return;
}

/**
 * \brief           Configure PWM registers for Up Down Count
 */
void ConfigEPwmUpDwn(volatile struct EPWM_REGS * ePWM, Uint16 period)
{
    EALLOW;

    // Time Base SubModule Registers
    ePWM->TBPRD = (Uint16)(period * 0.5f);      // PWM frequency = 1 / period
    ePWM->TBPHS.bit.TBPHS = 0;                  // Set Phase register to zero
    ePWM->TBCTR = 0;                            // clear TB counter
    ePWM->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // Symmetric
    ePWM->TBCTL.bit.PHSEN = TB_DISABLE;         // Phase loading disabled
    ePWM->TBCTL.bit.PRDLD = TB_SHADOW;
    ePWM->TBCTL.bit.SYNCOSEL   = TB_CTR_ZERO;   // sync "down-stream"
    ePWM->TBCTL.bit.HSPCLKDIV  = TB_DIV1;
    ePWM->TBCTL.bit.CLKDIV     = TB_DIV1;

    ePWM->CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    ePWM->CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    ePWM->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;   // load on CTR = Zero
    ePWM->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;   // load on CTR = Zero

    // Counter Compare Submodule Registers
    ePWM->CMPA.bit.CMPA = 0;                    // set duty 0% initially
    ePWM->CMPB.bit.CMPB = 0;                    // set duty 0% initially

    // Action Qualifier SubModule Registers
    ePWM->AQCTLA.bit.CAU = AQ_CLEAR;
    ePWM->AQCTLA.bit.CAD = AQ_SET;
    ePWM->AQCTLB.bit.CBU = AQ_SET;
    ePWM->AQCTLB.bit.CBD = AQ_CLEAR;

    // Active high complementary PWMs - Disable deadband
    ePWM->DBCTL.bit.OUT_MODE   = DB_DISABLE;

    EDIS;
}


/**
 * \brief           Configure PWM registers for Slave
 */
void ConfigEPwmSlave(volatile struct EPWM_REGS * ePWM)
{
    EALLOW;

    ePWM->TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    ePWM->TBCTL.bit.PHSEN    = TB_ENABLE;
    ePWM->TBPHS.bit.TBPHS    = 2;
    ePWM->TBCTL.bit.PHSDIR   = TB_UP;

    EDIS;
}


/**
 * \brief           Configure PWM registers to create SOC
 */
void ConfigEPwmSOC(volatile struct EPWM_REGS * ePWM)
{
    EALLOW;

    ePWM->ETSEL.bit.SOCASEL  = ET_CTR_PRD; // Select SOC from counter at ctr = PRD
    ePWM->ETPS.bit.SOCAPRD   = ET_1ST;     // Generate pulse on 1st even
    ePWM->ETSEL.bit.SOCAEN   = 1;          // Enable SOC on A group

    EDIS;
}

/**
 * \brief           Write ADC configurations and power up the ADC
 */
void ConfigureADC(void)
{
    EALLOW;

    //write configurations
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4

    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //power up the ADC
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);

    EDIS;
}


/**
 * \brief           Write ADC configurations and power up the ADC
 */
void ADCChannelSetup(void)
{
    EALLOW;

    // Ia  @ C2
    // ********************************
    AdccRegs.ADCSOC0CTL.bit.CHSEL     = 2;                     // SOC C0 will convert ADC C2
    AdccRegs.ADCSOC0CTL.bit.ACQPS     = 30;                    // sample window in SYSCLK cycles
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;                     // trigger on ePWM1 SOCA

    // Ib  @ B2
    // ********************************
    AdcbRegs.ADCSOC0CTL.bit.CHSEL     = 2;                     // SOC B0 will convert ADC B2
    AdcbRegs.ADCSOC0CTL.bit.ACQPS     = 30;                    // sample window in SYSCLK cycles
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;                     // trigger on ePWM1 SOCA

    // Ic  @ A2
    // ********************************
    AdcaRegs.ADCSOC0CTL.bit.CHSEL     = 2;                     // SOC A0 will convert ADC A2
    AdcaRegs.ADCSOC0CTL.bit.ACQPS     = 30;                    // sample window in SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;                     // trigger on ePWM1 SOCA

    // Ua  @ C3
    // ********************************
    AdccRegs.ADCSOC1CTL.bit.CHSEL     = 3;                     // SOC C1 will convert ADC C3
    AdccRegs.ADCSOC1CTL.bit.ACQPS     = 30;                    // sample window in SYSCLK cycles
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;                     // trigger on ePWM1 SOCA

    // Ub  @ B3
    // ********************************
    AdcbRegs.ADCSOC1CTL.bit.CHSEL     = 3;                     // SOC B1 will convert ADC B3
    AdcbRegs.ADCSOC1CTL.bit.ACQPS     = 30;                    // sample window in SYSCLK cycles
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;                     // trigger on ePWM1 SOCA

    // Uc  @ A3
    // ********************************
    AdcaRegs.ADCSOC1CTL.bit.CHSEL     = 3;                     // SOC A1 will convert ADC A3
    AdcaRegs.ADCSOC1CTL.bit.ACQPS     = 30;                    // sample window in SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;                     // trigger on ePWM1 SOCA

    // Udc  @ B14
    // ********************************
    AdcbRegs.ADCSOC2CTL.bit.CHSEL     = 14;                    // SOC B2 will convert ADC B14
    AdcbRegs.ADCSOC2CTL.bit.ACQPS     = 30;                    // sample window in SYSCLK cycles
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL   = 5;                     // trigger on ePWM1 SOCA

    EDIS;
}



/**
* \brief           Configurate EQEP for quadrature count mode
*/
void EQepRegsConfig(volatile struct EQEP_REGS * EQepRegs, Uint32 NPLS)
{

    EALLOW;

    // Unit Timer for 100Hz at 200 MHz SYSCLKOUT
    EQepRegs->QUPRD = (Uint32)(SYSTEM_FREQUENCY*1000000.0f*0.01f);

    EQepRegs->QDECCTL.bit.QSRC = 00;    // QEP quadrature count mode
    EQepRegs->QDECCTL.bit.QAP = 0;      // QEPA input polarity
    EQepRegs->QDECCTL.bit.QBP = 0;      // QEPB input polarity

    EQepRegs->QEPCTL.bit.FREE_SOFT = 2;
    // PCRM=00 mode - QPOSCNT reset on index event
    EQepRegs->QEPCTL.bit.PCRM = 00;

    EQepRegs->QEPCTL.bit.UTE = 1;         // Unit Timeout Enable
    EQepRegs->QEPCTL.bit.QCLM = 1;        // Latch on unit time out
    EQepRegs->QPOSMAX = NPLS;
    EQepRegs->QEPCTL.bit.QPEN = 1;        // QEP enable
    EQepRegs->QCAPCTL.bit.UPPS = 5;       // 1/32 for unit position
    EQepRegs->QCAPCTL.bit.CCPS = 6;       // 1/64 for CAP clock
    EQepRegs->QCAPCTL.bit.CEN = 1;        // QEP Capture Enable

    EDIS;
}
