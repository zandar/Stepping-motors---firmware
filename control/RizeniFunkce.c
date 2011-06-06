/**
  Project : Krokove motory
  Version : 2.0
  Date    : 29.8.2010
  Author  : Michal Vokac vokacmic@fel.cvut.cz
  Company : Student CVUT FEL
  Comments: Firmware for stepping motors controll board.

  Chip type		: ATmega8
  Clock frequency	: 8,000000 MHz
*/

#include "RizeniFunkce.h"
#include "motor.h"
#include "uart.h"

/** Zjisti stav optozavor a odesle na seriovou linkou */
void sendOptoStatus()
{
  unsigned char opto = 0;
  
  opto = (get_opto(0) == 0 ? (opto | OPTO_1_ECLIPSED) : (opto & OPTO_1_FREE));    /* informace opto 1 */
  opto = (get_opto(1) == 0 ? (opto | OPTO_2_ECLIPSED) : (opto & OPTO_2_FREE));    /* informace opto 2 */
  opto = (get_opto(2) == 0 ? (opto | OPTO_3_ECLIPSED) : (opto & OPTO_3_FREE));    /* informace opto 3 */
  opto = (get_opto(3) == 0 ? (opto | OPTO_4_ECLIPSED) : (opto & OPTO_4_FREE));    /* informace opto 4 */
      
  putchar(opto);
}

/** Otaceni motorem dle prijatych parametru */
void rotateMotor(struct rotParameters *rotation)
{   
  unsigned int i;
  
  motor_dir(rotation->direction, rotation->motorNumber);   /* nastavi smer otaceni dle pozadavku z PC */
  
  if (rotation->rotType == 0) {			/* otaceni po krocich */
    for (i = 0; i < rotation->steps; i++) {	/* otaci pozadovany pocet kroku */
      step_motor(rotation->speed, rotation->motorNumber);
    }
  }
  else {			/* otaceni po sektorech */
    if (get_opto(rotation->motorNumber) == OPTO_FREE) {		/* pokud je optozavora volna */
      while (get_opto(rotation->motorNumber) == OPTO_FREE) {	/* otaci dokud je volna na konec mezery + 1 krok */
        step_motor(rotation->speed, rotation->motorNumber);
      }
      
      step_motor(rotation->speed, rotation->motorNumber);
    }
    
    for (i = 0; i < rotation->steps; i++) {	/* pokud je zastinena */
      while (get_opto(rotation->motorNumber) == OPTO_ECLIPSED) {			/* otaci dokud je zastinena na zacatek mezery + 1 krok */
        step_motor(rotation->speed, rotation->motorNumber);
      }
      
      step_motor(rotation->speed, rotation->motorNumber);
      
      while (get_opto(rotation->motorNumber) == OPTO_FREE) {	/* pak dokud je volna na konec mezery  + 1 krok */
        step_motor(rotation->speed, rotation->motorNumber);
      }
      
      step_motor(rotation->speed, rotation->motorNumber);
    }
    
    motor_dir(!rotation->direction, rotation->motorNumber);	/* nastavi otaceni na opacnou stranu */
    
    for (i = 0; i < 8; i++) {	/* vrati se o par kroku zpet do stredu posledni mezery */
      step_motor(rotation->speed, rotation->motorNumber);
    }
  }
}

