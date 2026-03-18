#include <osbind.h>

#include <osbind.h>

/* Return the system timer longword (auto-incremented 70 times/sec).
 * Uses privileged mode briefly to read protected memory at 0x462.
 */
unsigned long get_time(void);
