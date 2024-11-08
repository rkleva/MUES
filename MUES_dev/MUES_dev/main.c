

/**
 * main.c
 */

#include "F28x_Project.h"
#include "board_setup.h"



int main(void)
{

    /* Main function for board initialization - calls and all functions implemented in board_setup.c */
    initialize_board();

    /* Loop forever */
    for(;;)
    {

        GPIO_WritePin(BLUE_LED_GPIO, 0);
        DELAY_US(1000000);
        GPIO_WritePin(BLUE_LED_GPIO, 1);
        DELAY_US(1000000);

        asm ("    NOP");

    }

}
