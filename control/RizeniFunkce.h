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

struct rotParameters;

struct rotParameters
{
  volatile unsigned char motorNumber;  /* cislo motoru pro otaceni */
  volatile unsigned char direction;    /* smer otaceni motorem, 1 = vlevo, 0 = vpravo */
  volatile unsigned char rotType;      /* zpusob otaceni motorem, 0 = po krocich, 1 = po sektorech */
  volatile unsigned int speed;         /* rychlost otaceni motorem */
  volatile unsigned int steps;         /* pocek kroku nebo sektoru o kolik se ma otacet */
};
    
void rotateMotor(struct rotParameters *rotation);

void sendOptoStatus(void);