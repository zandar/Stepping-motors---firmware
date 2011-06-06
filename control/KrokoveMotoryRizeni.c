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

#include <avr/io.h>
#define F_CPU	8000000UL	// 8 MHz
#include <util/delay.h>
#include <avr/interrupt.h>
#include "RizeniFunkce.h"
#include "uart.h"
                  

/** Global variables */
struct rotParameters rotation;

volatile unsigned char reciveCnt = 0;	/* citac poctu prijatych bajtu */
volatile char reciveData[7];	/* pole pro ulozeni poslanych bajtu */
unsigned int crc = 0;		/* kontrolni soucet komunikace */
volatile char rotate = 0;	/* signalizace od preruseni seriove linky ze se ma otacet motorem */

void decodeReciveData();
void test();

/** External interrupt INT0 service routine */
ISR(INT0_vect)
{
  /* Pozadavek na zastaveni motoru*/
  /* TODO nastavit watchdog a nechat resetovat procesor */
}

/** External Interrupt INT1 service routine */
ISR(INT1_vect)
{
  /* zastavit otaceni motorem-vynulovat vsechny promenne souvisejci s otacenim */
  //rotation.steps = 0;
}


/** UART interrupt service routine */
ISR(USART_RXC_vect)
{  
  if (reciveCnt == 0) {		/* pokud jde o prvni preruseni */
    reciveData[0] = getchar();	/* precte jeden byte z UART */
    
    switch (reciveData[0]) {	/* rozdeleni ukolu podle prijateho prvniho znaku - hlavicky komunikace */
      case 'H':			/* prijata hlavicka oznacujci pozadavek na otaceni motorem */
        reciveCnt++;
        return;
        break;
	
      case 'O':           /* hlavicka pozadujci odeslani stavu optozavor */
        _delay_ms(50);    /* bezpecnostni pauza, aplikace nestiha zpracovavat prichozi data */
        sendOptoStatus();
        break;    
    }

    /*pokud je hlavicka spatna nebo se nepozaduje dalsi zpracovani znaku z UART, vynuluje promenne a vrati se */
    reciveCnt = 0;
    reciveData[0] = 0;
    return;
  }
  else {    				/* dalsi preruseni pri prijmu zpravy*/
    if (reciveCnt == 6) {		/* pokud prislo posledni preruseni */
      int i = 0;
      reciveData[6] = getchar();	/* prijme posledni bajt */
      crc = 0;
      
      for (; i < 6; i++) {			/* provede kontrolni soucet vsech bajtu */
        crc += reciveData[i];
      }
      
      if (crc == reciveData[6]) {	/* pokud je kontrola OK, provest otaceni motorem a vratit informaci o poloze kotouce */
        puts("OK");
       // putchar('O');			/* odesle do PC info, ze prijeti probehlo OK a provede se otaceni */
       // putchar('K');
       // putchar(10);
        rotate = 1;			/* nastavi priznak pro otaceni - povoli dekodovani prijatych dat a provedeni otaceni */
      }              
      else {  		/* pokud kontrola neni OK, vrati info o chybe komunikace, otaceni se neprovede */
        puts("ER");
     //   putchar('E');	/* odesle do PC info, ze prijeti neprobehlo OK, otaceni se neprovede */
      //  putchar('R');
       // putchar(10);
        rotate = 0;	/* nastavi priznak zakazujci otaceni */
      }
      
      reciveCnt = 0;		/* vynuluje pocet prijatych znaku */
      reciveData[0] = 0;	/* vymaze zasobnik prijatych dat */
      return;  
    }
    
    /* prijme dalsi bajt z UART a inkrementuje citac preruseni */
    reciveData[reciveCnt] = getchar();
    reciveCnt++;    
  }
}

/** Metoda pro dekodovani prijatych dat a jejich ulozeni */
void decodeReciveData()
{
  rotation.motorNumber = (reciveData[1] >> 6);    /* ulozeni cisla ovladace */
  rotation.direction = !((reciveData[1] & 0x3F) >> 5);  /* ulozeni smeru otaceni */
  rotation.rotType = ((reciveData[1] & 0x1F) >> 4); /* ulozeni zpusobu otaceni */
  rotation.speed = reciveData[3];     /* ulozeni horniho bajtu rychlosti */
  rotation.speed = rotation.speed << 8;
  rotation.speed += reciveData[2];      /* ulozeni dolniho bajtu rychlosti */
  rotation.steps = reciveData[5];     /* ulozeni horniho bajtu kroku/sektoru */
  rotation.steps = rotation.steps << 8;
  rotation.steps += reciveData[4];      /* ulozeni dolniho bajtu kroku/sektoru */
}

/** MAIN */

int main(void)
{
  /**
    Port B initialization
    PB7=In
    PB6=In
    PB5=Out - MOTOR 3 , smer otaceni
    PB4=Out - MOTOR 3 , krokovaci impulsy
    PB3=In  - MOTOR 3 , optozavora
    PB2=Out - MOTOR 4 , smer otaceni
    PB1=Out - MOTOR 4 , krokovaci impulsy
    PB0=In  - MOTOR 4 , optozavora
    State7=T State6=T State5=0 State4=0 State3=T State2=0 State1=0 State0=T 
  */
  PORTB=0x09;
  DDRB=0x36;

  /** Port C initialization
    PC6=In  - RESET 
    PC5=Out - MOTOR 1 , smer otaceni
    PC4=Out - MOTOR 1 , krokovaci impulsy
    PC3=In  - MOTOR 1 , optozavora
    PC2=Out - MOTOR 2 , smer otaceni
    PC1=Out - MOTOR 2 , krokovaci impulsy
    PC0=In  - MOTOR 2 , optozavora
    State6=T State5=0 State4=0 State3=T State2=0 State1=0 State0=T 
  */
  PORTC=0x09;
  DDRC=0x36;

  /**
    Port D initialization
    PD7=In
    PD6=In - FTDI DSR
    PD5=In - FTDI DCD
    PD4=In - FTDI CTS
    PD3=In - FTDI DTR
    PD2=In - FTDI RTS
    PD1=In - FTDI RxD
    PD0=In - TFDI TxD
  */
  PORTD=0x00;
  DDRD=0x00;

  /**
    External Interrupts initialization
    INT0: On
    INT0 Mode: Rising Edge
    INT1: On
    INT1 Mode: Rising Edge
  */
  GICR|=0xC0;
  MCUCR=0x0F;
  GIFR=0xC0;

  /**
    USART initialization
    Communication Parameters: 8 Data, 2 Stop, No Parity
    USART Receiver: On
    USART Transmitter: On
    USART Mode: Asynchronous
    USART Baud Rate: 9600
  */
  UCSRA=0x00;
  UCSRB=0x98;
  UCSRC=0x8E;
  UBRRH=0x00;
  UBRRL=0x33;

  sei();	/* global enable interrupts */
  

  /** Hlavni cinnost programu */

  while (1) {     /* ceka v nekonecne smycce na preruseni ktere povoli vstup do tela */
    if (rotate) {
      decodeReciveData();
      rotateMotor(&rotation);
      
      _delay_ms(50);      /* bezpecnostni pauza, aplikace nestiha zpracovavat prichozi data pri kratsim cyklu */
      sendOptoStatus();   /* po skonceni otaceni odesle stav oprozavor */
      rotate = 0;         /* deaktivuje priznak otaceni */
    }    
  }
  return 0;
}
