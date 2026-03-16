#ifndef CLOCK_H
#define CLOCK_H

/* Return the system timer longword (auto-incremented 70 times/sec).
 * Uses privileged mode briefly to read protected memory at 0x462.
 */
unsigned long get_time(void);

#endif /* CLOCK_H */
