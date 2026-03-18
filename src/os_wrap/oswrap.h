#ifndef OSWRAP_H
#define OSWRAP_H

#include <stddef.h>

/* Minimal OS abstraction layer used by main loop.
 * Implemented as thin wrappers around TOS calls in the emulator build.
 */

/* Return non-zero if input is available */
int os_has_input(void);

/* Get input key code (caller should ensure input available) */
unsigned int os_get_input(void);

/* Return pointer to framebuffer base */
unsigned char *os_framebuffer_base(void);

#endif /* OSWRAP_H */
