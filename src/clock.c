#include "clock.h"
#include <osbind.h>

unsigned long get_time(void)
{
    volatile unsigned long *timer = (volatile unsigned long *)0x462UL;
    long prev = Super(0); /* enter privileged mode */
    unsigned long t = *timer;
    Super(prev); /* restore previous mode ASAP */
    return t;
}
