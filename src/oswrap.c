#include "oswrap.h"
#include <osbind.h>

int os_has_input(void)
{
    /* Cconis from osbind returns non-zero if char available */
    return Cconis();
}

unsigned int os_get_input(void)
{
    return Cnecin();
}

unsigned char *os_framebuffer_base(void)
{
    return (unsigned char *)Physbase();
}

void os_yield(void)
{
    /* No-op for now */
}
