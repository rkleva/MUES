/*
 * MotorControl.c
 *
 *  Created on: 14 Oct 2022
 *      Author: ZESA
 */


#include "F28x_Project.h"
#include "board_setup.h"
#include "math.h"


#include "settings.h"
#include "control_lib.h"


Uint32 IsrTicker = 0;

/*
 * Primjer deklaracije varijabli za citanje vrijednosti registara
 */
Uint16 Ia_regs = 0;
Uint16 Ib_regs = 0;
Uint16 Ic_regs = 0;
Uint16 Ua_regs = 0;
Uint16 Ub_regs = 0;
Uint16 Uc_regs = 0;
Uint16 Udc_regs = 0;

/*
 * Deklaracija varijabli za kalibraciju
 * offset_cal_total -> ukupan broj poziva prekidne funkcije za kalibraciju - promijeniti prema potrebi
 * offset_cal_idx -> trenutacni broj poziva prekidne funkcije za kalibraciju
 * offset_cal_done -> zastavica za odredivanje statusa kalibracije
 */
Uint16 offset_cal_total = 200;
Uint16 offset_cal_idx = 0;
int16 offset_cal_done = FALSE;

/*
 * Zastavica za potrebe funkcija inicijalizacije
 */
int16 init_vars_done = FALSE;

/*
 * Zastavica za ukljucenje tranzistora u gornjoj grani faze
 */
int16 enable_switch = FALSE;

/*
 * referentna vrijednost iz vanjskog sustava
 */
float32 ref_val = FALSE;

/*
 * Primjer ispitivanja funkcije za filtriranje signala.
 * Za potrebe primjera, filtriati ce se varijabla ref_val
 */

filter_t filt_example = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

/*
 * Deklaracija varijabli za upis perioda vodenja u registre PWM modula
 */
Uint16 CMP_pwm1 = 0;
Uint16 CMP_pwm2 = 0;
Uint16 CMP_pwm3 = 0;


/*********************************
 * Programski kod prekidne rutine:
 *********************************/
interrupt void MotorControlISR(void){

    // Verifying the ISR
    IsrTicker++;

    // Inicijalizacija varijabli:
    if (init_vars_done == FALSE){
        // U ovom dijelu koda se mogu napraviti inicijalizacije struktura

        // inicijalizacija filtra
        filter_init(&filt_example, 0.5f, CTRL_TS); // vremenska konstanta je 0.5 sekundi

        init_vars_done = TRUE;

        // Interrupt Acknowledge  i zavrsetak funkcije
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT1=1;
        PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
        return;
    }


    /*
    * U ovom dijelu koda se citaju vrijednosti ADC registara za mjerenje struja i napona.
    * Prikazan je primjer citanja vrijednosti registra i zapis vrijednosti u proizvoljno deklarirane pomocne varijable.
    * Prema potrebi, vrijednost registra se moze pohraniti u vlastitu strukturu ili varijablu umjesto u prikazane.
    *
    * Registri ADC-a podeseni su tijekom inicijalizacije sustava!
    * Nazivi proizvoljnih varijabli odgovaraju mjerenoj varijabli!
    * Obratiti paznju na nazive registara AdcXResu... ->
    * vrijednost X moze biti a, b ili c i odgovara koristenom ADC-u.
    */

    Ia_regs = AdccResultRegs.ADCRESULT0;
    Ib_regs = AdcbResultRegs.ADCRESULT0;
    Ic_regs = AdcaResultRegs.ADCRESULT0;
    Ua_regs = AdccResultRegs.ADCRESULT1;
    Ub_regs = AdcbResultRegs.ADCRESULT1;
    Uc_regs = AdcaResultRegs.ADCRESULT1;
    Udc_regs = AdcbResultRegs.ADCRESULT2;


    /*
    * KALIBRACIJA NULTE VRIJEDNOSTI:
    * U ovom dijelu koda potrebno je ocitavati vrijednosti registara mjerene struje kada struja ne tece
    * (Ia = Ib = Ic = 0) tijekom vise poziva prekidne rutine. Ocitane vrijednosti zapisane u registrima
    * je potrebno usrednjiti i pohraniti kao kalibriranu vrijednost nule potrebnu za izracun vrijednosti struje.
    *
    * Potrebno je obratiti paznju na format varijable u kojoj se zapisuje kalibrirane vrijednost nule.
    * U primjeru je postavljeno 200 poziva ISR-a, sto znaci da se 200 puta pribrojiti ocitana vrijednost
    * registra koja otprilike iznosi 2250. Dakle, varijabla mora moci pohraniti vrijednosti koja otprilike
    * iznosi 200*2250 = 450000, a za veci odabrani broj ukupnih poziva ISR-a i vise.
    *
    * Potrebno je kalibrirati samo nulte vrijednosti struje. Mjerenje napona je unipolarno stoga vrijenost
    * registra 0 odgovara vrijednosti 0 V.
    */
    if (!offset_cal_done){
        offset_cal_idx = offset_cal_idx + 1; // uvecanje brojaca poziva ISR-a tijekom kalibracije

        // mjesto predvideno za pohranu ocitanih vrijednosti registara:


        if(offset_cal_idx >= offset_cal_total){
            // mjesto predvideno za usrednjavanje,
            // odnosno kalibraciju nulte vrijednosti:


            // postavljanje zastavice da je kalibracija zavrsena
            offset_cal_idx = 0;
            offset_cal_done = TRUE;
        }

        // Interrupt Acknowledge i zavrsetak funkcije
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT1=1;
        PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
        return;
    }


    /*
     * Nakon zavrsetka kalibracije moze se izvrsavati preostali dio koda prekidne rutine.
     */

        // mjesto predvideno za izracun vrijednosti napona i struja na
        // temelju ocitanih vrijednosti registara:


    /*
     * Dio koda za ukljucenje tranzistora gornje grane jedne faze
     * Inicijalno, ukljuceni su svi tranzistori u donjim granima, odnosno:
     *      CMP_pwm1 = 0;
     *      CMP_pwm2 = 0;
     *      CMP_pwm2 = 0;
     *
     * U ovom primjeru, postavljanjem zastavice enable_switch u 1, ukljucuje se
     * samo gornji tranzistor u prvoj fazi (CMP_pwm1 = INV_PWM_TBPRD)
     * dok se upravljanje tranzistorima ostale dvije faze ne mijenja.
     *
     * Za potrebe ispitivanja, moguce je promijeniti redni broj faze u kojoj se
     * ukljucuje tranzistor u gornjoj grani
     */

    // ulaz u filtar
    filt_example.in = ref_val;
    // izvrsavanje filtra
    filter_calc(&filt_example);


    if(enable_switch){
        CMP_pwm1 = INV_PWM_TBPRD;
    }
    else {
        CMP_pwm1 = 0;
        CMP_pwm2 = 0;
        CMP_pwm3 = 0;
    }


    // Upis u registre PWM-a
    EPwm1Regs.CMPA.bit.CMPA = CMP_pwm1;
    EPwm2Regs.CMPA.bit.CMPA = CMP_pwm2;
    EPwm3Regs.CMPA.bit.CMPA = CMP_pwm3;

    // Interrupt Acknowledge
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1=1;
    PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;

}
