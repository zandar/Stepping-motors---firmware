#include <avr/io.h>
#include "uart.h"

/** Prijme znak z UART */
char getchar(void)
{
    char status;
    char data;

    while (1)
    {
        while (((status = UCSRA) & RX_COMPLETE) == 0);
        data = UDR;
        if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN)) == 0)
            return data;
    };
}

/** Odesle znak na UART */
void putchar(char c)
{
    while ((UCSRA & DATA_REGISTER_EMPTY) == 0);
    UDR = c;
}

/** Odesle retezec na UART */
void puts(char *str)
{
  char c;
  while (c = *str++)
    putchar(c);
  putchar(10);
}