/**
 * ikbd.h
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Vector installation and mouse/keyboard utilization.
 */
#include "../shared/dtypes.h"

#define IKBD_ISR 70

#define IKBD_CTRL_ADDR      0xFFFC00
#define IKBD_STATUS_ADDR    0xFFFC00
#define IKBD_RDR_ADDR       0xFFFC02

#define IKBD_CR_DIV64       0x02    /* CR0-CR1: clock divide by 64 */
#define IKBD_CR_8N1         0x14    /* CR2-CR4: 8 bits, no parity, 1 stop bit */
#define IKBD_CR_TX_NO_INT   0x00    /* CR5-CR6: RTS low, Tx int disabled */
#define IKBD_CR_NO_RX_INT   0x00    /* CR7: Rx interrupt disabled */
#define IKBD_CR_RX_INT      0x80    /* CR7: Rx interrupt enabled */

#define IKBD_POLLING_MODE   (IKBD_CR_DIV64 | IKBD_CR_8N1 | IKBD_CR_TX_NO_INT | IKBD_CR_NO_RX_INT)
#define IKBD_RESTORE_MODE   (IKBD_CR_DIV64 | IKBD_CR_8N1 | IKBD_CR_TX_NO_INT | IKBD_CR_RX_INT)

#define IKBD_SR_RDRF        0x01    /* Status bit 0: Receive Data Register Full */

#define ESC_SCANCODE        0x01
#define BREAK_BIT           0x80

#define IKBD_SR_IRQ         0x80    /* Status bit 7: IRQ pending */

#define SC_SPACE 0x39
#define SC_Q 0x10
#define SC_ENTER 0x1C

/**
 * Installs a new interrupt vector and returns the previous vector.
 * Caller is responsible for being in supervisor mode.
 *
 * Params:
 *     UINT16 vectorNum:
 *       The interrupt vector number to install
 *     Vector handler:
 *       The new interrupt handler to install
 *
 * Returns:
 *     Vector: The previous interrupt handler
 */
Vector installVector(UINT16 vectorNum, Vector handler);

/**
 * Default IKBD ISR placeholder.
 */
void ikbdisr(void);
