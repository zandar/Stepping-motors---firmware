/**
  Project : Krokove motory
  Version : 2.0
  Date    : 29.8.2010
  Author  : Michal Vokac vokacmic@fel.cvut.cz
  Company : Student CVUT FEL
  Comments: Firmware for stepping motors power board. 

  Chip type      : ATtiny13
  Clock frequency: 9,600000 MHz
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU	9600000UL	// 9.6 MHz

/**
  PINB.3 = motA
  PINB.4 = motB
*/

#define motA_set() PORTB |= (1 << PB3);		/* PINB.3 = 1 */
#define motA_clr() PORTB &= ~(1 << PB3);	/* PINB.3 = 0 */
#define motB_set() PORTB |= (1 << PB4);		/* PINB.4 = 1 */
#define motB_clr() PORTB &= ~(1 << PB4);	/* PINB.4 = 0 */

void toc();
unsigned int stav = 0;  /* obsahuje cislo aktualniho kroku */

/** External Interrupt 0 service routine */
ISR(INT0_vect)
{
  toc();
}

/** Metoda pro otoceni motoru o jeden krok, volana v preruseni INT 0 */
void toc()
{
  if ((PINB & (1 << PB0)) == 1) { /* PB0, volba smeru otaceni motoru, 0 = po smeru, 1 = proti smeru */
    /* otaceni proti smeru */
    switch (++stav) {  /* nastavi stav o jedna vetsi nez je aktualni */
      case 4: /* prechod ze stavu 3 na stav 0 */
      case 0: 
	motA_set();	/* p3 = 1 */
	motB_set();	/* p4 = 1 */
	stav = 0;
	break;
      case 1:
	motA_set();	/* p3 = 1 */
	motB_clr();	/* p4 = 0 */
	stav = 1;
	break;
      case 2:
	motA_clr();	/* p3 = 0 */
	motB_clr();	/* p4 = 0 */
	stav = 2;
	break;
      case 3:
	motA_clr();	/* p3 = 0 */
	motB_set();	/* p4 = 1 */
	stav = 3;
	break;   
    }
  }
  else {
    /* otaceni po smeru */
    switch (stav - 1) {  /* nastavi stav o jedna mensi nez je aktualni */
      case 0: 
	motA_set();	/* p3 = 1 */
	motB_set();	/* p4 = 1 */
	stav=0;
	break;
      case 1:
	motA_set();	/* p3 = 1 */
	motB_clr();	/* p4 = 0 */
	stav = 1;
	break;
      case 2:
	motA_clr();	/* p3 = 0 */
	motB_clr();	/* p4 = 0 */
	stav = 2;
	break;
      case -1:    /* prechod ze stavu 0 do stavu 3 */
      case 3:
	motA_clr();	/* p3 = 0 */
	motB_set();	/* p4 = 1 */
	stav = 3;
	break;
    }
  }
}

int main(void)
{ 
  /**
    Port B initialization
    P5 = In
    P4 = Out
    P3 = Out
    P2 = In
    P1 = In
    P0 = In 
  */
  PORTB = 0x03;
  DDRB = 0x1C;

  /**
    External Interrupt(s) initialization
    INT0: On
    INT0 Mode: Falling Edge
  */
  GIMSK = 0x40;
  MCUCR = 0x00;
  GIFR = 0x40;

  /**
    Global enable interrupts
  */
  sei();
  
  /**
    Motor init
  */
  motA_set();	/* p3 = 1 */
  motB_set();	/* p4 = 1 */
  stav = 0;

  /* ceka ve smycce na preruseni krokovacim pulsem */
  while (1);
  
  return 0;
}
