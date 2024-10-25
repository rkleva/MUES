#include "control_lib.h"
#include "settings.h"
extern float32 ref_val;

// Initalization function for filter
void filter_init (filter_t *filter, float tf, float t_samp)
{
    filter->t_samp = t_samp;
    filter->tf = tf;
    filter->coeff = filter->t_samp / (filter->t_samp + filter->tf);
}


// Calculation function for filter
void filter_calc (filter_t *filter)
{
    filter->out = filter->out * (1 - filter->coeff) + (filter->coeff * filter->in);
}

float calculate_voltage(int *adc_vrijednost) {
    float uu_mj = (3 * (*adc_vrijednost)) / 4096.0f;
    float napon = uu_mj * (1 / A );

    return napon;

}

float calculate_current(int *adc_vrijednost) {

    float u_mj = (3  * (*adc_vrijednost))/4096.0f;
    float struja = (u_mj - 1.65) / (10 * R_sh);

    return struja;


}

float calculate_voltage_pu(float *value) {

    return *value / U_B;

}

float calculate_current_pu(float *value) {


    return *value / I_B;

}

float check_ref_val (float *value) {

    if( ref_val <= fabs(*value) ) {

        return 0;
    } else {

        return 1 //Error code da nesto ne stima;
    }
}

float calculate_duty_cycle (float *ref_val, float *Udc) {

    if(*Udc == 0) {
        return 2 //Error code da je Udc trenutno nula;
    }

    return (*ref_val / *Udc) * 5000;

}

float calculate_duty_cycle2 (float *ref_val , float *Udc) {
    if(*Udc == 0) {

        return 3 //Error code da je Udc trenutno nula;
    }



    return (5000 * (*ref_val) + 5000) / (2 * (*Udc));

}
