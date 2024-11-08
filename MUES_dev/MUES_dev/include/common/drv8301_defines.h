//----------------------------------------------------------------------------------
//	FILE:			drv8301_defs.c
//
//	Description:	Contains DRV8301 register definitions
//
//	Version: 		1.0
//
//  Target:  		TMS320F28377S
//
//----------------------------------------------------------------------------------
//  Copyright Texas Instruments © 2004-2015
//----------------------------------------------------------------------------------
//  Revision History:
//----------------------------------------------------------------------------------
//  Date	  | Description / Status
//----------------------------------------------------------------------------------
// 4 Nov 2015 - DRV8301 register definitions
//----------------------------------------------------------------------------------

#ifndef DRV8301_DEFS_H_
#define DRV8301_DEFS_H_

#include "F2837xD_device.h"        // F2837xD Headerfile Include File

/*****************************************************************************/
// Register Addresses
/*****************************************************************************/
//DRV8301 Register Addresses
#define STAT_REG_1_ADDR		0x00
#define STAT_REG_2_ADDR		0x01
#define CNTRL_REG_1_ADDR	0x02
#define CNTRL_REG_2_ADDR	0x03


enum {
	drv8301_gateCurrent_1700mA = 0,
	drv8301_gateCurrent_700mA = 1,
	drv8301_gateCurrent_250mA = 2
};

enum {
	drv8301_gate_reset = 1
};

enum {
	drv8301_pwmMode_6 = 0,
	drv8301_pwmMode_3 = 1
};

enum {
	drv8301_ocp_cbc = 0,
	drv8301_ocp_sd = 1,
	drv8301_ocp_rptonly = 2,
	drv8301_ocp_disable = 3
};

enum {
	drv8301_oc_60mV = 0,
	drv8301_oc_68mV = 1,
	drv8301_oc_76mV = 2,
	drv8301_oc_86mV = 3,
	drv8301_oc_97mV = 4,
	drv8301_oc_109mV = 5,
	drv8301_oc_123mV = 6,
	drv8301_oc_138mV = 7,
	drv8301_oc_155mV = 8,
	drv8301_oc_175mV = 9,
	drv8301_oc_197mV = 10,
	drv8301_oc_222mV = 11,
	drv8301_oc_250mV = 12,
	drv8301_oc_282mV = 13,
	drv8301_oc_317mV = 14,
	drv8301_oc_358mV = 15,
	drv8301_oc_403mV = 16,
	drv8301_oc_454mV = 17,
	drv8301_oc_511mV = 18,
	drv8301_oc_576mV = 19,
	drv8301_oc_648mV = 20,
	drv8301_oc_730mV = 21,
	drv8301_oc_822mV = 22,
	drv8301_oc_926mV = 23,
	drv8301_oc_1043mV = 24,
	drv8301_oc_1175mV = 25,
	drv8301_oc_1324mV = 26,
	drv8301_oc_1491mV = 27,
	drv8301_oc_1679mV = 28,
	drv8301_oc_1892mV = 29,
	drv8301_oc_2131mV = 30,
	drv8301_oc_2400mV = 31
};


enum {
	drv8301_octw_ocot = 0,
	drv8301_octw_ot = 1,
	drv8301_octw_oc = 2
};

enum {
	drv8301_gain_10 = 0,
	drv8301_gain_20 = 1,
	drv8301_gain_40 = 2,
	drv8301_gain_80 = 3
};

enum {
	drv8301_dc_cal_noshort = 0,
	drv8301_dc_cal_short = 1
};

enum {
	drv8301_oc_toff_cbc = 0,
	drv8301_oc_toff_offtimecntl = 1
};


#define  DRV8301_DEFAULTS {                        \
		0,    /* DRV8301_stat_reg1;    */          \
		0,    /* DRV8301_stat_reg2;    */          \
		0,    /* DRV8301_cntrl_reg1;   */          \
		0,    /* DRV8301_cntrl_reg2;   */          \
                                                   \
		0,    /* Uint16 readCntrl1,    */          \
		0,    /* readCntrl2,           */          \
		0,    /* DRV_fault = 0,        */          \
}

//=============================================================================

#endif /* DRV8301_DEFS_H_ */
