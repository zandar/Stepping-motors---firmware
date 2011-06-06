/**
  Project : Krokove motory
  Version : 2.0
  Date    : 29.8.2010
  Author  : Michal Vokac vokacmic@fel.cvut.cz
  Company : Student CVUT FEL
  Comments: Firmware for stepping motors controll board.

  Chip type   : ATmega8
  Clock frequency : 8,000000 MHz
*/

#include <avr/io.h>
#define F_CPU 8000000UL // 8 MHz
#include <util/delay.h>
#include "motor.h"

/** Zjisti stav konkretni optozavory */
char get_opto(char mot)
{
  switch (mot) {
    case 0:
      return OPTO_1;
      break;
      
    case 1:
      return OPTO_2;
      break;
      
    case 2:
      return OPTO_3;
      break;
      
    case 3:
      return OPTO_4;
      break;
      
    default:
      return 0xFF;
  }
}

/** Nastavi smer otaceni dle pozadavku z PC */
motor_dir(char dir,char mot)
{
  switch (mot) {
    case 0:
      mot1_dir(dir);
      break;
      
    case 1:
      mot2_dir(dir);
      break;
      
    case 2:
      mot3_dir(dir);
      break;
      
    case 3:
      mot4_dir(dir);
      break;
  }
}

/** Otoci motorem 1 o jeden krok */
void step_motor(unsigned int speed, char mot)
{
  switch (mot) {
    case 0:
      mot1_set();
      _delay_ms(2);
      mot1_clr();
      _delay_ms(speed);
      break;
      
    case 1:
      mot2_set();
      _delay_ms(2);
      mot2_clr();
      _delay_ms(speed);
      break;
      
    case 2:
      mot3_set();
      _delay_ms(2);
      mot3_clr();
      _delay_ms(speed);
      break;
      
    case 3:
      mot4_set();
      _delay_ms(2);
      mot4_clr();
      _delay_ms(speed);
      break;
  }
}
