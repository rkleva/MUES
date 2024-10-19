/*
 * control_lib.h
 *
 *  Created on: 2023
 *      Author: ZESA
 */

#ifndef CONTROL_LIB_H_
#define CONTROL_LIB_H_

#define VREFLO 0
#define VREFHI 3
#define R_sh 0.01
#define A  (4.99/(4.99 + 34.8))
#define U_B 24.0f
#define I_B 2.0f



/*-----------------------------------------------------------------------------
Strucutre definitions
-----------------------------------------------------------------------------*/

// Filter structure
typedef struct {
    float in;                                   /*!< \brief Unfiltered input signal */
    float out;                                  /*!< \brief Filtered output signal */
    float tf;                                   /*!< \brief Filter time constant */
    float t_samp;                               /*!< \brief Sampling time */
    float coeff;                                /*!< \brief Filter coefficient */
 } filter_t;



/*--------------------------------------------------------------------------------
Prototype functions
----------------------------------------------------------------------------------*/
void filter_init (filter_t *filter, float tf, float t_samp);
void filter_calc (filter_t *filter);

#endif //CONTROL_LIB_H_
