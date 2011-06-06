#define RXB8  1
#define TXB8  0
#define UPE   2
#define OVR   3
#define FE    4
#define UDRE  5
#define RXC   7

#define FRAMING_ERROR (1 << FE)
#define PARITY_ERROR  (1 << UPE)
#define DATA_OVERRUN  (1 << OVR)
#define DATA_REGISTER_EMPTY (1 << UDRE)
#define RX_COMPLETE (1 << RXC)

/** Prijme znak z UART */
char getchar(void);

/** Odesle znak na UART */
void putchar(char c);