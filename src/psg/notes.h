/**
 * notes.h
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Constant values for playing specific notes to make music easier to write,
 * based on the formula TP = 125000 / fT (fMaster = 2MHz, SEL low) from the
 * YM2149 manual. 
 */

/* Octave 2 */
#define D2S 4011  /* D# 77.78Hz  */
#define E2  3787  /* E  82.41Hz  */
#define F2  3574  /* F  87.31Hz  */
#define F2S 3373  /* F# 92.50Hz  */
#define G2  3184  /* G  98.00Hz  */
#define G2S 3005  /* G# 103.83Hz */
#define A2  2837  /* A  110.00Hz */
#define A2S 2678  /* A# 116.54Hz */
#define B2  2528  /* B  123.47Hz */

/* Octave 3 */
#define C3  2386  /* C  130.81Hz */
#define C3S 2252  /* C# 138.59Hz */
#define D3  2126  /* D  146.83Hz */
#define D3S 2006  /* D# 155.56Hz */
#define E3  1893  /* E  164.81Hz */
#define F3  1787  /* F  174.61Hz */
#define F3S 1687  /* F# 185.00Hz */
#define G3  1592  /* G  196.00Hz */
#define G3S 1503  /* G# 207.65Hz */
#define A3  1419  /* A  220.00Hz */
#define A3S 1339  /* A# 233.08Hz */
#define B3  1264  /* B  246.94Hz */

/* Octave 4 */
#define C4  1193  /* C  261.63Hz */
#define C4S 1126  /* C# 277.18Hz */
#define D4  1063  /* D  293.66Hz */
#define D4S 1003  /* D# 311.13Hz */
#define E4   947  /* E  329.63Hz */
#define F4   893  /* F  349.23Hz */
#define F4S  843  /* F# 369.99Hz */
#define G4   796  /* G  392.00Hz */
#define G4S  751  /* G# 415.30Hz */
#define A4   710  /* A  440.00Hz */
#define A4S  670  /* A# 466.16Hz */
#define B4   632  /* B  493.88Hz */

/* Octave 5 */
#define C5   597  /* C  523.25Hz */
#define C5S  563  /* C# 554.37Hz */
#define D5   532  /* D  587.33Hz */
#define D5S  502  /* D# 622.25Hz */
#define E5   474  /* E  659.26Hz */
#define F5   447  /* F  698.46Hz */
#define F5S  422  /* F# 739.99Hz */
#define G5   398  /* G  783.99Hz */
#define G5S  376  /* G# 830.61Hz */
#define A5   355  /* A  880.00Hz */
#define A5S  335  /* A# 932.33Hz */
#define B5   316  /* B  987.77Hz */

/* Octave 6 */
#define C6   298  /* C  1046.50Hz */
#define C6S  282  /* C# 1108.73Hz */
#define D6   266  /* D  1174.66Hz */
#define D6S  251  /* D# 1244.51Hz */
#define E6   237  /* E  1318.51Hz */
#define F6   224  /* F  1396.91Hz */
#define F6S  211  /* F# 1479.98Hz */
#define G6   199  /* G  1567.98Hz */
#define G6S  188  /* G# 1661.22Hz */
#define A6   178  /* A  1760.00Hz */
#define A6S  168  /* A# 1864.66Hz */
#define B6   158  /* B  1975.53Hz */

/* Octave 7 */
#define C7   149  /* C  2093.00Hz */
#define C7S  141  /* C# 2217.46Hz */
#define D7   133  /* D  2349.32Hz */
#define D7S  125  /* D# 2489.02Hz */
#define E7   118  /* E  2637.02Hz */
#define F7   112  /* F  2793.83Hz */
#define F7S  105  /* F# 2959.96Hz */
#define G7   100  /* G  3135.96Hz */
#define G7S   94  /* G# 3322.44Hz */
#define A7    89  /* A  3520.00Hz */
#define A7S   84  /* A# 3729.31Hz */
#define B7    79  /* B  3951.07Hz */

/* Special note value for a rest (silence) */
#define REST 0