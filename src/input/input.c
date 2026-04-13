/**
 * input.c
 *
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Implementation of the high-level input module. This handles the dependency
 * injection or wrapping of OS-specific keyboard routines like Cconis and
 * Cnecin.
 */
#include "input.h"

volatile UINT8 *const IKBD_control = IKBD_CTRL_ADDR;
volatile const UINT8 *const IKBD_status = IKBD_STATUS_ADDR;
volatile const SCANCODE *const IKBD_RDR = IKBD_RDR_ADDR;

/* ISR state machine */
#define STATE_INPUT_AWAIT 0
#define STATE_MOUSE_X_AWAIT 1
#define STATE_MOUSE_Y_AWAIT 2

/* Circular queue */
#define QUEUE_SIZE 256

#define MOUSE_MASK 0xF8 /* If it's a mouse byte, high 5 bits are set */

SCANCODE queue[QUEUE_SIZE];
int queue_head;
int queue_tail;

/* Mouse state */
int mouse_x;
int mouse_y;
UINT8 mouse_buttons;
int isr_state;
static Vector original_vector;

void enqueue(SCANCODE sc)
{
    int next = (queue_tail + 1) % QUEUE_SIZE;
    if (next != queue_head)
    { /* drop if full */
        queue[queue_tail] = sc;
        queue_tail = next;
    }
}

static int queue_empty(void)
{
    return queue_head == queue_tail;
}

void init_input(void)
{
    long old_ssp;

    queue_head = 0;
    queue_tail = 0;
    mouse_x = 0;
    mouse_y = 0;
    mouse_buttons = 0;
    isr_state = STATE_INPUT_AWAIT;

    old_ssp = Super(0);
    original_vector = installVector(IKBD_ISR, ikbdisr);
    *IKBD_control = IKBD_RESTORE_MODE;
    Super(old_ssp);
}

void restore_input(void) {
    long old_ssp;

    old_ssp = Super(0);
    installVector(IKBD_ISR, original_vector);
    Super(old_ssp);
}

int has_input(void)
{
    return queue_empty() == 0 ? TRUE : FALSE;
}

SCANCODE get_input(void) {
    SCANCODE sc = 0;

    if (has_input() == TRUE) {
        sc = queue[queue_head];
        queue_head = (queue_head + 1) % QUEUE_SIZE;
    }

    return sc;
}

SCANCODE read_scancode(void)
{
    long old_ssp;
    SCANCODE sc;

    old_ssp = Super(0);
    *IKBD_control = IKBD_POLLING_MODE;
    while ((*IKBD_status & IKBD_SR_RDRF) == 0)
        ;
    sc = *IKBD_RDR;
    *IKBD_control = IKBD_RESTORE_MODE;
    Super(old_ssp);

    return sc;
}

void handle_ikbd_byte(void) {
    SCANCODE byte;

    if ((*IKBD_status & IKBD_SR_IRQ) == 0) {
        return;
    }

    byte = *IKBD_RDR;

    if (isr_state == STATE_INPUT_AWAIT) {
        if ((byte & MOUSE_MASK) == MOUSE_MASK) {
            mouse_buttons = byte & 0x03; /* Bottom two bits for buttons */
            isr_state = STATE_MOUSE_X_AWAIT;
        } else {
            enqueue(byte);
        }
    } else if (isr_state == STATE_MOUSE_X_AWAIT) {
        mouse_x += (signed char)byte;
        isr_state = STATE_MOUSE_Y_AWAIT;
    } else {
        mouse_y += (signed char)byte;
        isr_state = STATE_INPUT_AWAIT;
    }
}
