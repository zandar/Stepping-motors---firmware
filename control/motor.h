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

#define mot1_dir(dir) PORTC = (dir == 0 ? (PINC & ~(1 << PINC5)) : (PINC | (1 << PINC5)));  /* PORTC.5 = direction */
#define mot2_dir(dir) PORTC = (dir == 0 ? (PINC & ~(1 << PINC2)) : (PINC | (1 << PINC2)));  /* PORTC.2 = direction */
#define mot3_dir(dir) PORTB = (dir == 0 ? (PINB & ~(1 << PINB5)) : (PINB | (1 << PINB5)));  /* PORTB.5 = direction */
#define mot4_dir(dir) PORTB = (dir == 0 ? (PINB & ~(1 << PINB2)) : (PINB | (1 << PINB2)));  /* PORTB.2 = direction */

#define mot1_set() PORTC |= (1 << PINC4)  /* PORTC.4 = 1 */
#define mot1_clr() PORTC &= ~(1 << PINC4) /* PORTC.4 = 0 */
#define mot2_set() PORTC |= (1 << PINC1)  /* PORTC.1 = 1 */
#define mot2_clr() PORTC &= ~(1 << PINC1) /* PORTC.1 = 0 */
#define mot3_set() PORTB |= (1 << PINB4)  /* PORTB.4 = 1 */
#define mot3_clr() PORTB &= ~(1 << PINB4) /* PORTB.4 = 0 */
#define mot4_set() PORTB |= (1 << PINB1)  /* PORTB.1 = 1 */
#define mot4_clr() PORTB &= ~(1 << PINB1) /* PORTB.1 = 0 */

#define OPTO_1 (PINC & (1 << PINC3))  /* informace opto 1 */
#define OPTO_2 (PINC & (1 << PINC0))  /* informace opto 2 */
#define OPTO_3 (PINB & (1 << PINB3))  /* informace opto 3 */
#define OPTO_4 (PINB & (1 << PINB0))  /* informace opto 4 */

/* stav optozavor  1 = volno, 0 = zastineno */

#define OPTO_1_FREE     0xFE
#define OPTO_1_ECLIPSED 0x01
#define OPTO_2_FREE     0xFD
#define OPTO_2_ECLIPSED 0x02
#define OPTO_3_FREE     0xFB
#define OPTO_3_ECLIPSED 0x04
#define OPTO_4_FREE     0xF7
#define OPTO_4_ECLIPSED 0x08

#define OPTO_FREE 1
#define OPTO_ECLIPSED 0

void step_motor(unsigned int, char);
void motor_dir(char, char);
char get_opto(char);