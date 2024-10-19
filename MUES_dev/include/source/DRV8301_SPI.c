//============================================================================
//============================================================================
//
// FILE:	DRV8301_SPI.c
//
// TITLE:	DRV8301 SPI comm functions
// 
// Version: 1.0 	
//
// Date: 	24 May 2011
//
//============================================================================
//============================================================================
#include "DRV8301_SPI.h"
#include "settings.h"


/*****************************************************************************/
// Initialize the SPI peripheral
/*****************************************************************************/
void DRV8301SPIInit(volatile struct SPI_REGS *s)
{
	s->SPICCR.bit.SPISWRESET = 0; 		// Put SPI in reset state
	s->SPICCR.bit.SPICHAR = 0xF;		// 16-bit character
	s->SPICCR.bit.SPILBK = 0;     		// loopback off
	s->SPICCR.bit.CLKPOLARITY = 0;  	// Rising edge without delay

	s->SPICTL.bit.SPIINTENA = 0;		// disable SPI interrupt
	s->SPICTL.bit.TALK = 1;				// enable transmission
	s->SPICTL.bit.MASTER_SLAVE = 1;		// master
	s->SPICTL.bit.CLK_PHASE = 0;    	// Rising edge without delay
	s->SPICTL.bit.OVERRUNINTENA = 0;	// disable reciever overrun interrupt
	
	// s->SPIBRR = 0;						// SPICLK = LSPCLK / 4 (max SPICLK)
	s->SPIBRR.bit.SPI_BIT_RATE = 19;        // SPICLK = LSPCLK / 4 (max SPICLK)

	s->SPICCR.bit.SPISWRESET=1;  		// Enable SPI
}

/*****************************************************************************/
// Read from a DRV8301 Register
/*****************************************************************************/
Uint16 DRV8301SPIRead(volatile struct SPI_REGS *s, Uint16 address)
{
	union DRV8301_SPI_WRITE_WORD_REG w;
	volatile Uint16 dummy;
	
	w.bit.R_W = 1;							//we are initiating a read
	w.bit.ADDRESS = address;				//load the address
	w.bit.DATA = 0;							//dummy data;
	
	s->SPITXBUF = w.all;					//send out the data
	
	while(s->SPISTS.bit.INT_FLAG == 0);		//wait for the packet to complete
	
	dummy = s->SPIRXBUF;					//dummy read to clear the INT_FLAG bit
	
	w.bit.R_W = 1;							//we are initiating a read
	w.bit.ADDRESS = address;				//load the address
	w.bit.DATA = 0;							//dummy data;
	
	s->SPITXBUF = w.all;					//send out the data
	
	while(s->SPISTS.bit.INT_FLAG == 0);		//wait for the packet to complete
	
	dummy = s->SPIRXBUF;					//dummy read to clear the INT_FLAG bit

	return(dummy);
	
}

/*****************************************************************************/
// Write to a DRV8301 Register
// SPI writes always clock out the data in Status Register 1.
// Since it's available we'll return the status from this function
/*****************************************************************************/
Uint16 DRV8301SPIWrite(volatile struct SPI_REGS *s, Uint16 address, Uint16 data)
{
	union DRV8301_SPI_WRITE_WORD_REG w;
	volatile Uint16 stat_reg1;
	
	w.bit.R_W = 0;							//we are initiating a write
	w.bit.ADDRESS = address;				//load the address
	w.bit.DATA = data;						//data to be written;
	
	s->SPITXBUF = w.all;					//send out the data
	
	while(s->SPISTS.bit.INT_FLAG == 0);		//wait for the packet to complete
	
	stat_reg1 = s->SPIRXBUF;				//read returned value of Status Register 1 and clear the INT_FLAG bit
	
	return(stat_reg1);
	
}


void InitDrvData(DRV8301_Vars *drv)
{

    drv->cntrl_reg1.all = 0;
    drv->cntrl_reg1.bit.GATE_CURRENT = drv8301_gateCurrent_700mA;
    drv->cntrl_reg1.bit.PWM_MODE     = drv8301_pwmMode_3;
    drv->cntrl_reg1.bit.OC_MODE      = drv8301_ocp_sd;
    drv->cntrl_reg1.bit.OC_ADJ_SET   = drv8301_oc_123mV;

    drv->cntrl_reg2.all = 0;
    drv->cntrl_reg2.bit.OCTW_SET   = drv8301_octw_ot;
    drv->cntrl_reg2.bit.OC_TOFF    = drv8301_oc_toff_cbc;
    drv->cntrl_reg2.bit.GAIN       = (DRV_GAIN == 20) ? drv8301_gain_20 :
                                     (DRV_GAIN == 40) ? drv8301_gain_40 :
                                     (DRV_GAIN == 80) ? drv8301_gain_80 : drv8301_gain_10;
    drv->cntrl_reg2.bit.DC_CAL_CH1 = drv8301_dc_cal_noshort;
    drv->cntrl_reg2.bit.DC_CAL_CH2 = drv8301_dc_cal_noshort;

    return;
}


void InitDRV8301(volatile struct SPI_REGS *s, DRV8301_Vars *drv, Uint16 clkPin, Uint16 scsPin){
    Uint16 temp, *ptr1, *ptr2;

    //================================================================================
    // write all DRV8301 control registers, and read all control and status registers
    //================================================================================
    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 0);               // Enable SCS
    drv->stat_reg1.all = DRV8301SPIWrite(s, CNTRL_REG_1_ADDR, drv->cntrl_reg1.all);       // write conrol reg 1, read status reg 1
    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 1);               // Disable SCS
    DELAY_US(5);


    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 0);               // Enable SCS
    drv->stat_reg1.all = DRV8301SPIWrite(s, CNTRL_REG_2_ADDR, drv->cntrl_reg2.all);       // write conrol reg 2, read status reg 1
    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 1);               // Disable SCS
    DELAY_US(5);


    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 0);               // Enable SCS
    drv->stat_reg2.all = DRV8301SPIRead(s, STAT_REG_2_ADDR) & 0x80;  // read status reg 2
    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 1);               // Disable SCS
    DELAY_US(5);


    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 0);               // Enable SCS
    drv->readCntrl1 = DRV8301SPIRead(s, CNTRL_REG_1_ADDR) & 0x7ff;   // read control reg 1
    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 1);               // Disable SCS
    DELAY_US(5);


    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 0);               // Enable SCS
    drv->readCntrl2 = DRV8301SPIRead(s, CNTRL_REG_2_ADDR) & 0x7ff;   // read control reg 2
    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 1);               // Disable SCS
    DELAY_US(5);

    //===============================================
    // DRV Fault diagnostics - check Status reg 1
    //===============================================
    ptr1 = (Uint16 *) &(drv->stat_reg1.all);
    ptr2 = (Uint16 *) &(drv->readCntrl1);
    drv->DRV_fault = 0;

    // verify status registers
    for (temp = 0; temp<= 1; temp++)
    {
        if (*ptr1++)
            drv->DRV_fault |= 1 << temp;
    }

    // verify control registers
    for (; temp<= 3; temp++)
    {
        if (*ptr1++ != *ptr2++)
            drv->DRV_fault |= 1 << temp;
    }

    return;
}

void ReadDRV8301Stat(volatile struct SPI_REGS *s, DRV8301_Vars *drv, Uint16 clkPin, Uint16 scsPin){

    //================================================================================
    // write all DRV8301 control registers, and read all control and status registers
    //================================================================================
    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 0);               // Enable SCS
    drv->stat_reg1.all = DRV8301SPIRead(s, STAT_REG_1_ADDR);  // read status reg 2
    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 1);               // Disable SCS
    DELAY_US(5);

    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 0);               // Enable SCS
    drv->stat_reg2.all = DRV8301SPIRead(s, STAT_REG_2_ADDR) & 0x80;  // read status reg 2
    while(!clkPin) ;                        // wait until clock goes low
    GPIO_WritePin(scsPin, 1);               // Disable SCS
    DELAY_US(5);


    return;
}
