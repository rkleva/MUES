/*
 * MotorControl.c
 *
 *  Created on: 14 Oct 2022
 *      Author: ZESA
 */


#include "settings.h"
#include "control_lib.h"

Uint32 IsrTicker = 0;

/*
 * Primjer deklaracije varijabli za citanje vrijednosti registara
 */
int Ia_regs = 0;
int Ib_regs = 0;
int Ic_regs = 0;
int Ua_regs = 0;
int Ub_regs = 0;
int Uc_regs = 0;
int Udc_regs = 0;

Uint32 Ia_cal = 0;
Uint32 Ib_cal = 0;
Uint32 Ic_cal = 0;

float Ia = 0;
float Ib = 0;
float Ic = 0;
float Ua = 0;
float Ub = 0;
float Uc = 0;
float Udc = 0;

float Ia_pu = 0;
float Ib_pu = 0;
float Ic_pu = 0;
float Ua_pu = 0;
float Ub_pu = 0;
float Uc_pu = 0;
float Udc_pu = 0;

float Ia_0 = 0;
float Ib_0 = 0;
float Ic_0 = 0;

float provjera = 0;
float calculated_TBPRD_A = 0;
float calculated_TBPRD_B = 0;
int real_TBPRD_A = 0;
int real_TBPRD_B = 0;
int vrsta_modulacije = 0; //Sluzit ce za modulaciju jedne ili obje grane pretvaraca


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
float32 ref_val = 12;

/*
 * Deklaracija varijabli za upis perioda vodenja u registre PWM modula
 */
Uint16 CMP_pwm1 = 0;
Uint16 CMP_pwm2 = 0;
Uint16 CMP_pwm3 = 0;

// Variables for Datalog module - ne koristi se u Matlabu
#ifdef _LAUNCHXL_F28379D
/*
 * u ovom dijelu koda ce se inicijalizirati varijable
 * funkcije za snimanje podataka na 4. lab vjezbi
 */
#endif

/*
 * Primjer ispitivanja funkcije za filtriranje signala.
 * Za potrebe primjera, filtriati ce se varijabla ref_val
 */

filter_t filt_example = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};


/*********************************
 * Programski kod prekidne rutine:
 *********************************/
#ifdef _LAUNCHXL_F28379D
    interrupt void MotorControlISR(void){

#else
    void MotorControlISR(uint16_T MatlabAdcxRegs[6], real_T* MatlabValuesSI, real_T matlab_enable, real_T matlab_ref, uint16_T* CMP_PWM){
        enable_switch = (int16)(matlab_enable);
        ref_val = (float)(matlab_ref);
#endif


    // Verifying the ISR
    IsrTicker++;

    // Inicijalizacija varijabli:
    if (init_vars_done == FALSE){
        // U ovom dijelu koda se mogu napraviti inicijalizacije struktura

        // inicijalizacija filtra
        filter_init(&filt_example, 0.5f, CTRL_TS); // vremenska konstanta je 0.5 sekundi

        init_vars_done = TRUE;
#ifdef _LAUNCHXL_F28379D
        // Interrupt Acknowledge
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT1=1;
        PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
#endif
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

#ifdef _LAUNCHXL_F28379D    
    Ia_regs = AdccResultRegs.ADCRESULT0;
    Ib_regs = AdcbResultRegs.ADCRESULT0;
    Ic_regs = AdcaResultRegs.ADCRESULT0;
    Ua_regs = AdccResultRegs.ADCRESULT1;
    Ub_regs = AdcbResultRegs.ADCRESULT1;
    Uc_regs = AdcaResultRegs.ADCRESULT1;
    Udc_regs = AdcbResultRegs.ADCRESULT2;
#else
    Ia_regs = MatlabAdcxRegs[0];
    Ib_regs = MatlabAdcxRegs[1];
    Ic_regs = MatlabAdcxRegs[2];
    Ua_regs = MatlabAdcxRegs[3];
    Ub_regs = MatlabAdcxRegs[4];
    Uc_regs = MatlabAdcxRegs[5];
    Udc_regs = MatlabAdcxRegs[6];
#endif

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
       Ia_cal += Ia_regs;
       Ib_cal += Ib_regs;
       Ic_cal += Ic_regs;

        if(offset_cal_idx >= offset_cal_total){
            // mjesto predvideno za usrednjavanje,
            // odnosno kalibraciju nulte vrijednosti:
            Ia_cal = (Ia_cal / offset_cal_total);
            Ib_cal = (Ib_cal / offset_cal_total);
            Ic_cal = (Ic_cal / offset_cal_total);


                // postavljanje zastavice da je kalibracija zavrsena
                            offset_cal_idx = 0;
                            offset_cal_done = TRUE;


        }
#ifdef _LAUNCHXL_F28379D
        // Interrupt Acknowledge
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT1=1;
        PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
#endif
        return;
    }


    /*
     * Nakon zavrsetka kalibracije moze se izvrsavati preostali dio koda prekidne rutine.
     */

        // mjesto predvideno za izracun vrijednosti napona i struja na
        // temelju ocitanih vrijednosti registara:




            //Efektivna nula
            Ia_0 = calculate_current(&Ia_cal);
            Ib_0 = calculate_current(&Ib_cal);
            Ic_0 = calculate_current(&Ic_cal);
            //

            Ia = calculate_current(&Ia_regs) - Ia_0;
            Ib = calculate_current(&Ib_regs) - Ib_0;
            Ic = calculate_current(&Ic_regs) - Ic_0;


            Ia_pu = calculate_current_pu(&Ia);
            Ib_pu = calculate_current_pu(&Ib);
            Ic_pu = calculate_current_pu(&Ic);

            Ua = calculate_voltage(&Ua_regs);
            Ub = calculate_voltage(&Ub_regs);
            Uc = calculate_voltage(&Uc_regs);
            Udc = calculate_voltage(&Udc_regs);


            Ua_pu = calculate_voltage_pu(&Ua);
            Ub_pu = calculate_voltage_pu(&Ub);
            Uc_pu = calculate_voltage_pu(&Uc);
            Udc_pu = calculate_voltage_pu(&Udc);


            /* Tek ako je sweitch upaljen mogu nastaviti s racunanjem */
            if(enable_switch) {

            }

            /* Kod za provjeru ref_val vrijednosti */
            provjera = check_ref_val(&ref_val, &Udc);

            ref_val = provjera;

            /* Kod za izracun iznosa koji se upisuje u CMP_pwm1 */
            //Prvo je potrebno provjeriti koji se nacin upravljanja koristi

            if(vrsta_modulacije == 0) {        //Upravljanje samo jednom granom



                if(ref_val < 0) {
                    //Onda ne smijem paliti fazu A nego samo fazu B
                    calculated_TBPRD_B = calculate_duty_cycle(&ref_val, &Udc);
                    real_TBPRD_B = floor(calculated_TBPRD_B);

                } else {


                    calculated_TBPRD_A = calculate_duty_cycle(&ref_val, &Udc);
                    if(calculated_TBPRD_A == 2) {  //Dogodila se greška
                        while(1);
                    }
                    real_TBPRD_A = floorf(calculated_TBPRD_A);
                }

            } else if(vrsta_modulacije == 1) {  //Upravljanje sa dvije grane



                calculated_TBPRD_A = calculate_duty_cycle2(&ref_val, &Udc);
                real_TBPRD_A = floorf(calculated_TBPRD_A);

                real_TBPRD_B = INV_PWM_TBPRD - real_TBPRD_A;


            }









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

//    if(enable_switch){
//        CMP_pwm1 = real_TBPRD_A;
//        CMP_pwm2 = real_TBPRD_B;
//    }
//    else {
//        CMP_pwm1 = 0;
//        CMP_pwm2 = 0;
//        CMP_pwm3 = 0;
//    }


    /*
     * Nakon sto se odradio glavni dio posla prekidne rutine, rezultati izracuna mogu
     * se prikazati u CCS funkcijom za snimanje signala ili u Matlab-u
     */
#ifdef _LAUNCHXL_F28379D
    /*
     * u ovom dijelu koda ce se pridjeliti varijable
     * funkcije za snimanje podataka na 4. lab vjezbi
     */
#else
    MatlabValuesSI[0] = filt_example.out; // prikaz na izlazu
    MatlabValuesSI[1] = 0.0f;
    MatlabValuesSI[2] = 0.0f;
    MatlabValuesSI[3] = 0.0f;
    MatlabValuesSI[4] = 0.0f;
    MatlabValuesSI[5] = 0.0f;
    MatlabValuesSI[6] = 0.0f;
#endif


    // Upis u registre PWM-a
#ifdef _LAUNCHXL_F28379D
    if(enable_switch) {


        if(ref_val < 0) {
            EPwm1Regs.CMPA.bit.CMPA = CMP_pwm1;
            EPwm2Regs.CMPA.bit.CMPA = real_TBPRD_B;
            EPwm3Regs.CMPA.bit.CMPA = CMP_pwm3;

        } else {

        EPwm1Regs.CMPA.bit.CMPA = real_TBPRD_A;
        EPwm2Regs.CMPA.bit.CMPA = CMP_pwm2;
        EPwm3Regs.CMPA.bit.CMPA = CMP_pwm3;
        }
        if(vrsta_modulacije == 1) {

        EPwm1Regs.CMPA.bit.CMPA = real_TBPRD_A;
        EPwm2Regs.CMPA.bit.CMPA = real_TBPRD_B;
        EPwm3Regs.CMPA.bit.CMPA = CMP_pwm3;
        }



    } else {
        EPwm1Regs.CMPA.bit.CMPA = CMP_pwm1;
        EPwm2Regs.CMPA.bit.CMPA = CMP_pwm2;
        EPwm3Regs.CMPA.bit.CMPA = CMP_pwm3;


    }
    // Interrupt Acknowledge
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1=1;
    PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
#else
    CMP_PWM[0] = CMP_pwm1;
    CMP_PWM[1] = CMP_pwm2;
    CMP_PWM[2] = CMP_pwm3;
#endif
}
