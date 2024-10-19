//============================================================================
//============================================================================
//
// FILE:	DRV8301_SPI.h
//
// TITLE:	Header file for DRV8301 SPI comm functions
// 
// Version: 1.0 	
//
// Date: 	24 May 2011
//
//============================================================================
//============================================================================


#include "F2837xD_device.h"        // F2837xD Headerfile Include File
#include "drv8301_defines.h"

// DRV8301 SPI Input Data bit definitions:
struct  DRV8301_SPI_WRITE_WORD_BITS {       // bit    	description
   Uint16 DATA:11;          				// 10:0		FIFO reset
   Uint16 ADDRESS:4;        				// 14:11	Enhancement enable
   Uint16 R_W:1;          					// 15		R/W
};

union DRV8301_SPI_WRITE_WORD_REG {
   Uint16               				all;
   struct DRV8301_SPI_WRITE_WORD_BITS	bit;
};

//=============================================================================
// DRV8301 SPI Status Register 1 bit definitions:
struct  DRV8301_STATUS_REG_1_BITS {     // bit      description
   Uint16 FETLC_OC:1;                   // 0        Phase C, low-side FET OC
   Uint16 FETHC_OC:1;                   // 1        Phase C, high-side FET OC
   Uint16 FETLB_OC:1;                   // 2        Phase B, low-side FET OC
   Uint16 FETHB_OC:1;                   // 3        Phase B, high-side FET OC
   Uint16 FETLA_OC:1;                   // 4        Phase A, low-side FET OC
   Uint16 FETHA_OC:1;                   // 5        Phase A, high-side FET OC
   Uint16 OTW:1;                        // 6        Over-temperature warning
   Uint16 OTSD:1;                       // 7        Over-temperature shut down
   Uint16 PVDD_UV:1;                    // 8        PVDD < 6V
   Uint16 GVDD_UV:1;                    // 9        GVDD < 8V
   Uint16 FAULT:1;                      // 10       FAULTn pin is asserted
   Uint16 Reserved:5;                   // 15:11
};

typedef union  {
   Uint16                           all;
   struct DRV8301_STATUS_REG_1_BITS bit;
} DRV8301_STATUS_REG_1;


//==============================================
// DRV8301 SPI Status Register 2 bit definitions:
struct  DRV8301_STATUS_REG_2_BITS {     // bit      description
   Uint16 DEVICE_ID:4;                  // 3:0      Device ID
   Uint16 Reserved1:3;                  // 6:4
   Uint16 GVDD_OV:1;                    // 7        GVDD > 16V
   Uint16 Reserved2:8;                  // 15:8
};

typedef union  {
   Uint16                           all;
   struct DRV8301_STATUS_REG_2_BITS bit;
} DRV8301_STATUS_REG_2;


//==============================================
// DRV8301 SPI Control Register 1 bit definitions:
struct  DRV8301_CONTROL_REG_1_BITS {    // bit      description
   Uint16 GATE_CURRENT:2;               // 1:0      Gate driver peak current
   Uint16 GATE_RESET:1;                 // 2        Reset all latched faults
   Uint16 PWM_MODE:1;                   // 3        Three or six pwm inputs
   Uint16 OC_MODE:2;                    // 5:4      over-current mode,
   Uint16 OC_ADJ_SET:5;                 // 10:6     Set Vds trip point for OC
   Uint16 Reserved:5;                   // 15:11
};


typedef union  {
   Uint16                               all;
   struct DRV8301_CONTROL_REG_1_BITS    bit;
} DRV8301_CONTROL_REG_1;



//==============================================
// DRV8301 SPI Control Register 2 bit definitions:
struct  DRV8301_CONTROL_REG_2_BITS {    // bit      description
   Uint16 OCTW_SET:2;                   // 1:0      Report OT and / or OC
   Uint16 GAIN:2;                       // 3:2      Gain of shunt amplifier
   Uint16 DC_CAL_CH1:1;                 // 4        Shunt amp 1 connect/ disconnect
   Uint16 DC_CAL_CH2:1;                 // 5        Shunt amp 2 connect/ disconnect
   Uint16 OC_TOFF:1;                    // 6        CBC / off time control @ OC
   Uint16 Reserved:9;                   // 15:7
};

typedef union  {
   Uint16                               all;
   struct DRV8301_CONTROL_REG_2_BITS    bit;
} DRV8301_CONTROL_REG_2;


//=============================================================================

typedef struct  {    // DRV8301 parameters
    DRV8301_STATUS_REG_1   stat_reg1;
    DRV8301_STATUS_REG_2   stat_reg2;
    DRV8301_CONTROL_REG_1  cntrl_reg1;
    DRV8301_CONTROL_REG_2  cntrl_reg2;

    // DRV8301 variables
    Uint16 readCntrl1,
           readCntrl2,
           DRV_fault;
} DRV8301_Vars;


/***************************************************************************************************/
//function prototypes
/***************************************************************************************************/
void DRV8301SPIInit(volatile struct SPI_REGS *s);
void InitDrvData(DRV8301_Vars *drv);
void InitDRV8301(volatile struct SPI_REGS *s, DRV8301_Vars *drv, Uint16 clkPin, Uint16 scsPin);
void ReadDRV8301Stat(volatile struct SPI_REGS *s, DRV8301_Vars *drv, Uint16 clkPin, Uint16 scsPin);
Uint16 DRV8301SPIRead(volatile struct SPI_REGS *s, Uint16 address);
Uint16 DRV8301SPIWrite(volatile struct SPI_REGS *s, Uint16 address, Uint16 data);

