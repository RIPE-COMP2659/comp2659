#include <stdio.h>
#include <osbind.h>

#define BYTES_PER_SCREEN 32000

void disable_cursor();
void fill_screen(long *base, char pattern);

int main()
{
	long *base = Physbase();

	disable_cursor(); /* hide blinking text cursor */

	fill_screen(base, -1); /* set screen to all black */
	Cnecin();			   /* wait for key press, don't echo */

	fill_screen(base, 0); /* set screen to all white */
	Cnecin();

	return 0;
}

void disable_cursor()
{
	printf("\033f");
	fflush(stdout);
}

void fill_screen(long *base, char pattern)
{
	register int i = 0;
	register long *loc = base;

	while (i++ < BYTES_PER_SCREEN / 4)
		*(loc++) = pattern;
}
