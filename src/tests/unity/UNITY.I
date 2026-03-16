# 1 "tests/unity/unity.c" 1
# 1 "tests/unity/unity.h" 1
# 1 "tests/unity/unityi.h" 1
# 1 "C:/c68/include/setjmp.h" 1
# 1 "C:/c68/include/compiler.h" 1
# 5 "C:/c68/include/setjmp.h" 2
# 22 "C:/c68/include/setjmp.h"
typedef long jmp_buf[14];





typedef jmp_buf sigjmp_buf;

 int	sigsetjmp  (sigjmp_buf env, int savemask) ;
 int	siglongjmp (sigjmp_buf env, int val) ;
# 44 "C:/c68/include/setjmp.h"
 int	setjmp	(jmp_buf) ;
 void	longjmp	(jmp_buf, int) ;
# 16 "tests/unity/unityi.h" 2
# 1 "C:/c68/include/math.h" 1
# 62 "C:/c68/include/math.h"
typedef struct cmplx {
    double real;
    double imag;
} COMPLEX;


typedef enum  {
    DOMAIN      = 1,
    SING        = 2,
    OVERFLOW    = 3,
    UNDERFLOW   = 4,
    TLOSS       = 5,
    PLOSS       = 6
} exception_type;

struct exception {
	exception_type	type;
	const char	*name;
	double		arg1;
	double		arg2;
	double		retval;
};
# 101 "C:/c68/include/math.h"
extern const double _infinitydf;
# 128 "C:/c68/include/math.h"
 double sin	(double) ;
 double cos	(double) ;
 double tan	(double) ;
 double asin	(double) ;
 double	acos	(double) ;
 double atan	(double) ;
 double atan2	(double, double) ;
 double sinh	(double) ;
 double cosh	(double) ;
 double tanh	(double) ;
 double atanh	(double) ;
 double exp	(double) ;
 double log	(double) ;
 double log10	(double) ;
 double sqrt	(double) ;
 double hypot   (double, double) ;
 double pow	(double, double) ;
 double fabs	(double) ;
 double ceil	(double) ;
 double floor	(double) ;
 double rint	(double) ;
 double fmod	(double, double) ;

 double ldexp	(double, int) ;
 double frexp	(double, int *) ;
 double modf	(double, double *) ;


 double acosh	(double) ;
 double asinh	(double) ;
# 164 "C:/c68/include/math.h"
 double dabs	(double) ;


 double copysign	(double, double) ;
 int matherr	(struct exception *) ;
 double cabs	(COMPLEX) ;
 COMPLEX cmult	(COMPLEX, COMPLEX) ;
 COMPLEX csqrt	(COMPLEX) ;

 COMPLEX clog	(COMPLEX) ;

 COMPLEX cacos	(COMPLEX) ;
 COMPLEX cadd	(COMPLEX,COMPLEX) ;
 COMPLEX casin	(COMPLEX) ;
 COMPLEX catan	(COMPLEX) ;
 COMPLEX ccosh	(COMPLEX) ;
 COMPLEX crcp	(COMPLEX) ;
 COMPLEX csinh	(COMPLEX) ;
 COMPLEX ctan	(COMPLEX) ;
 COMPLEX ctanh	(COMPLEX) ;
 COMPLEX cexp	(COMPLEX) ;
 COMPLEX ccos	(COMPLEX) ;
 COMPLEX csin	(COMPLEX) ;
 COMPLEX cdiv	(COMPLEX, COMPLEX) ;
 COMPLEX csubt	(COMPLEX,COMPLEX) ;

 int pmlcfs	(int, int) ;
 int pmlcnt	(void) ;
 int pmlerr	(int) ;
 int pmllim	(int) ;
 int pmlsfs	(int, int) ;
 double poly	(int, double *, double) ;
# 20 "tests/unity/unityi.h" 2
# 1 "C:/c68/include/stddef.h" 1
# 19 "C:/c68/include/stddef.h"
typedef unsigned long  size_t;




typedef long  ptrdiff_t;




typedef char  wchar_t;
# 24 "tests/unity/unityi.h" 2
# 1 "C:/c68/include/limits.h" 1
# 41 "tests/unity/unityi.h" 2
# 170 "tests/unity/unityi.h"
    typedef unsigned char   UNITY_UINT8;
    typedef unsigned int    UNITY_UINT16;
    typedef unsigned long   UNITY_UINT32;
    typedef signed char     UNITY_INT8;
    typedef signed int      UNITY_INT16;
    typedef signed long     UNITY_INT32;
# 194 "tests/unity/unityi.h"
    typedef UNITY_UINT32 UNITY_UINT;
    typedef UNITY_INT32  UNITY_INT;
# 263 "tests/unity/unityi.h"
typedef float  UNITY_FLOAT;
# 306 "tests/unity/unityi.h"
  typedef UNITY_FLOAT UNITY_DOUBLE;
# 1 "C:/c68/include/stdio.h" 1
# 69 "C:/c68/include/stdio.h"
typedef	struct
	{
	long		_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	unsigned int	_flag;
	int		_file;
	long		_bsiz;
	unsigned char	_ch;
	}
	FILE;


typedef unsigned long fpos_t;
# 92 "C:/c68/include/stdio.h"
extern	FILE	_iob[];
# 108 "C:/c68/include/stdio.h"
 char *	ctermid	(char *) ;




 int	remove	(const char *) ;
 int	rename	(const char *, const char *) ;
 char *	tmpnam	(char *) ;
 FILE *	tmpfile	(void) ;

 int	fclose	(FILE *) ;
 int	fflush	(FILE *) ;

 FILE *	fopen	(const char *, const char *) ;
 FILE *	freopen	(const char *, const char *, FILE *) ;

 void	setbuf	(FILE *, char *) ;
 int	setvbuf	(FILE *, char *, int, size_t) ;
# 132 "C:/c68/include/stdio.h"
 int  fscanf  (FILE *, const char *, ...) ;
 int  scanf   (const char *, ...) ;
 int  sscanf  (const char *, const char *, ...) ;


 int	fprintf	(FILE *, const char *, ...) ;
 int	printf	(const char *, ...) ;
 int	sprintf	(char *, const char *, ...) ;

 int 	vfprintf (FILE *, const char *, char * ) ;
 int 	vprintf	 (const char *, char * ) ;
 int 	vsprintf (char *, const char *, char * ) ;

 int	vscanf  (const char *, char * ) ;
 int	vfscanf (FILE *, const char *, char * ) ;
 int	vsscanf	(const char *, const char *, char * ) ;


 int	fgetc	(FILE *) ;
 char	*fgets	(char *, int, FILE *) ;
 char	*gets	(char *) ;
 int	fputc	(int c, FILE *) ;
 int	fputs	(const char *, FILE *) ;
 int	puts	(const char *) ;

 size_t	fread	(void *, size_t, size_t, FILE *) ;
 size_t	fwrite	(const void *, size_t, size_t, FILE *) ;

 int	fgetpos	(FILE *, fpos_t *) ;
 int	fsetpos	(FILE *, fpos_t *) ;

 int	fseek	(FILE *, long, int) ;
 long	ftell	(FILE *) ;
 void	rewind	(FILE *) ;

 void	perror	(const char *) ;


 FILE	*fdopen	(int, const char *) ;


 FILE *	fopenp	(const char *, const char *) ;
 int 	fungetc	(int, FILE *) ;
 int	pclose	(FILE *) ;
 FILE *	popen	(const char *, const char *) ;
 void	setlinebuf	(FILE *) ;

 void	_binmode	(int) ;
 long 	getl	(FILE *) ;
 long 	putl	(long, FILE *) ;
 short 	getw	(FILE *) ;
 short 	putw	(short, FILE *) ;
 void	_getbuf	(FILE *fp) ;
# 192 "C:/c68/include/stdio.h"
 int	_filbuf	(FILE *) ;
# 329 "tests/unity/unityi.h" 2
# 451 "tests/unity/unityi.h"
typedef void (*UnityTestFunction)(void);
# 458 "tests/unity/unityi.h"
typedef enum
{
    UNITY_DISPLAY_STYLE_INT      = (16  / 8) + (0x10) ,
    UNITY_DISPLAY_STYLE_INT8     = 1 + (0x10) ,
    UNITY_DISPLAY_STYLE_INT16    = 2 + (0x10) ,
    UNITY_DISPLAY_STYLE_INT32    = 4 + (0x10) ,




    UNITY_DISPLAY_STYLE_UINT     = (16  / 8) + (0x20) ,
    UNITY_DISPLAY_STYLE_UINT8    = 1 + (0x20) ,
    UNITY_DISPLAY_STYLE_UINT16   = 2 + (0x20) ,
    UNITY_DISPLAY_STYLE_UINT32   = 4 + (0x20) ,




    UNITY_DISPLAY_STYLE_HEX8     = 1 + (0x40) ,
    UNITY_DISPLAY_STYLE_HEX16    = 2 + (0x40) ,
    UNITY_DISPLAY_STYLE_HEX32    = 4 + (0x40) ,




    UNITY_DISPLAY_STYLE_CHAR     = 1 + (0x80)  + (0x10) ,

    UNITY_DISPLAY_STYLE_UNKNOWN
} UNITY_DISPLAY_STYLE_T;

typedef enum
{
    UNITY_EQUAL_TO         = 0x1,
    UNITY_GREATER_THAN     = 0x2,
    UNITY_GREATER_OR_EQUAL = 0x2 + UNITY_EQUAL_TO,
    UNITY_SMALLER_THAN     = 0x4,
    UNITY_SMALLER_OR_EQUAL = 0x4 + UNITY_EQUAL_TO,
    UNITY_NOT_EQUAL        = 0x8
} UNITY_COMPARISON_T;


typedef enum UNITY_FLOAT_TRAIT
{
    UNITY_FLOAT_IS_NOT_INF       = 0,
    UNITY_FLOAT_IS_INF,
    UNITY_FLOAT_IS_NOT_NEG_INF,
    UNITY_FLOAT_IS_NEG_INF,
    UNITY_FLOAT_IS_NOT_NAN,
    UNITY_FLOAT_IS_NAN,
    UNITY_FLOAT_IS_NOT_DET,
    UNITY_FLOAT_IS_DET,
    UNITY_FLOAT_INVALID_TRAIT
} UNITY_FLOAT_TRAIT_T;


typedef enum
{
    UNITY_ARRAY_TO_VAL = 0,
    UNITY_ARRAY_TO_ARRAY,
    UNITY_ARRAY_UNKNOWN
} UNITY_FLAGS_T;
# 531 "tests/unity/unityi.h"
struct UNITY_STORAGE_T
{
    const char* TestFile;
    const char* CurrentTestName;
# 541 "tests/unity/unityi.h"
    const char* CurrentDetail1;
    const char* CurrentDetail2;


    UNITY_UINT  CurrentTestLineNumber;
    UNITY_UINT  NumberOfTests;
    UNITY_UINT  TestFailures;
    UNITY_UINT  TestIgnores;
    UNITY_UINT  CurrentTestFailed;
    UNITY_UINT  CurrentTestIgnored;





    jmp_buf AbortFrame;

};

extern struct UNITY_STORAGE_T Unity;





void UnityBegin(const char* filename);
int  UnityEnd(void);
void UnitySetTestFile(const char* filename);
void UnityConcludeTest(void);


void UnityDefaultTestRun(UnityTestFunction Func, const char* FuncName, const int FuncLineNum);
# 644 "tests/unity/unityi.h"
void UnityPrint(const char* string);





void UnityPrintLen(const char* string, const UNITY_UINT32 length);
void UnityPrintMask(const UNITY_UINT mask, const UNITY_UINT number);
void UnityPrintIntNumberByStyle(const UNITY_INT number, const UNITY_DISPLAY_STYLE_T style);
void UnityPrintUintNumberByStyle(const UNITY_UINT number, const UNITY_DISPLAY_STYLE_T style);
void UnityPrintNumber(const UNITY_INT number_to_print);
void UnityPrintNumberUnsigned(const UNITY_UINT number);
void UnityPrintNumberHex(const UNITY_UINT number, const char nibbles_to_print);


void UnityPrintFloat(const UNITY_DOUBLE input_number);
# 670 "tests/unity/unityi.h"
void UnityAssertEqualIntNumber(const UNITY_INT expected,
                               const UNITY_INT actual,
                               const char* msg,
                               const UNITY_UINT  lineNumber,
                               const UNITY_DISPLAY_STYLE_T style);

void UnityAssertEqualUintNumber(const UNITY_UINT expected,
                                const UNITY_UINT actual,
                                const char* msg,
                                const UNITY_UINT  lineNumber,
                                const UNITY_DISPLAY_STYLE_T style);

void UnityAssertIntGreaterOrLessOrEqualNumber(const UNITY_INT threshold,
                                              const UNITY_INT actual,
                                              const UNITY_COMPARISON_T compare,
                                              const char *msg,
                                              const UNITY_UINT  lineNumber,
                                              const UNITY_DISPLAY_STYLE_T style);

void UnityAssertUintGreaterOrLessOrEqualNumber(const UNITY_UINT threshold,
                                               const UNITY_UINT actual,
                                               const UNITY_COMPARISON_T compare,
                                               const char *msg,
                                               const UNITY_UINT  lineNumber,
                                               const UNITY_DISPLAY_STYLE_T style);

void UnityAssertEqualIntArray( const void*  expected,
 const void*  actual,
                              const UNITY_UINT32 num_elements,
                              const char* msg,
                              const UNITY_UINT  lineNumber,
                              const UNITY_DISPLAY_STYLE_T style,
                              const UNITY_FLAGS_T flags);

void UnityAssertBits(const UNITY_INT mask,
                     const UNITY_INT expected,
                     const UNITY_INT actual,
                     const char* msg,
                     const UNITY_UINT  lineNumber);

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_UINT  lineNumber);

void UnityAssertEqualStringLen(const char* expected,
                            const char* actual,
                            const UNITY_UINT32 length,
                            const char* msg,
                            const UNITY_UINT  lineNumber);

void UnityAssertEqualStringArray( const void*  expected,
                                  const char** actual,
                                  const UNITY_UINT32 num_elements,
                                  const char* msg,
                                  const UNITY_UINT  lineNumber,
                                  const UNITY_FLAGS_T flags);

void UnityAssertEqualMemory( const void*  expected,
 const void*  actual,
                             const UNITY_UINT32 length,
                             const UNITY_UINT32 num_elements,
                             const char* msg,
                             const UNITY_UINT  lineNumber,
                             const UNITY_FLAGS_T flags);

void UnityAssertIntNumbersWithin(const UNITY_UINT delta,
                                 const UNITY_INT expected,
                                 const UNITY_INT actual,
                                 const char* msg,
                                 const UNITY_UINT  lineNumber,
                                 const UNITY_DISPLAY_STYLE_T style);

void UnityAssertUintNumbersWithin(const UNITY_UINT delta,
                                  const UNITY_UINT expected,
                                  const UNITY_UINT actual,
                                  const char* msg,
                                  const UNITY_UINT  lineNumber,
                                  const UNITY_DISPLAY_STYLE_T style);

void UnityAssertNumbersArrayWithin(const UNITY_UINT delta,
 const void*  expected,
 const void*  actual,
                                   const UNITY_UINT32 num_elements,
                                   const char* msg,
                                   const UNITY_UINT  lineNumber,
                                   const UNITY_DISPLAY_STYLE_T style,
                                   const UNITY_FLAGS_T flags);


 void UnityFail(const char* message, const UNITY_UINT  line);
 void UnityIgnore(const char* message, const UNITY_UINT  line);





void UnityMessage(const char* message, const UNITY_UINT  line);


void UnityAssertFloatsWithin(const UNITY_FLOAT delta,
                             const UNITY_FLOAT expected,
                             const UNITY_FLOAT actual,
                             const char* msg,
                             const UNITY_UINT  lineNumber);

void UnityAssertFloatsNotWithin(const UNITY_FLOAT delta,
                                const UNITY_FLOAT expected,
                                const UNITY_FLOAT actual,
                                const char* msg,
                                const UNITY_UINT  lineNumber);

void UnityAssertGreaterOrLessFloat(const UNITY_FLOAT threshold,
                                   const UNITY_FLOAT actual,
                                   const UNITY_COMPARISON_T compare,
                                   const char* msg,
                                   const UNITY_UINT  linenumber);

void UnityAssertWithinFloatArray(const UNITY_FLOAT delta,
 const UNITY_FLOAT* expected,
 const UNITY_FLOAT* actual,
                                 const UNITY_UINT32 num_elements,
                                 const char* msg,
                                 const UNITY_UINT  lineNumber,
                                 const UNITY_FLAGS_T flags);

void UnityAssertFloatSpecial(const UNITY_FLOAT actual,
                             const char* msg,
                             const UNITY_UINT  lineNumber,
                             const UNITY_FLOAT_TRAIT_T style);
# 839 "tests/unity/unityi.h"
 const void*  UnityNumToPtr(const UNITY_INT num, const UNITY_UINT8 size);

 const void*  UnityFloatToPtr(const float num);
# 851 "tests/unity/unityi.h"
extern const char UnityStrOk[];
extern const char UnityStrPass[];
extern const char UnityStrFail[];
extern const char UnityStrIgnore[];

extern const char UnityStrErrFloat[];
extern const char UnityStrErrDouble[];
extern const char UnityStrErr64[];
extern const char UnityStrErrShorthand[];
# 22 "tests/unity/unity.h" 2
# 32 "tests/unity/unity.h"
void setUp(void);
void tearDown(void);
# 41 "tests/unity/unity.h"
void suiteSetUp(void);
int suiteTearDown(int num_failures);
# 53 "tests/unity/unity.h"
void resetTest(void);
void verifyTest(void);
# 8 "tests/unity/unity.c" 2
# 24 "tests/unity/unity.c"
struct UNITY_STORAGE_T Unity;
# 32 "tests/unity/unity.c"
const char UnityStrOk[]                            = "OK";
const char UnityStrPass[]                          = "PASS";
const char UnityStrFail[]                          = "FAIL";
const char UnityStrIgnore[]                        = "IGNORE";

static const char UnityStrNull[]                   = "NULL";
static const char UnityStrSpacer[]                 = ". ";
static const char UnityStrExpected[]               = " Expected ";
static const char UnityStrWas[]                    = " Was ";
static const char UnityStrGt[]                     = " to be greater than ";
static const char UnityStrLt[]                     = " to be less than ";
static const char UnityStrOrEqual[]                = "or equal to ";
static const char UnityStrNotEqual[]               = " to be not equal to ";
static const char UnityStrElement[]                = " Element ";
static const char UnityStrByte[]                   = " Byte ";
static const char UnityStrMemory[]                 = " Memory Mismatch.";
static const char UnityStrDelta[]                  = " Values Not Within Delta ";
static const char UnityStrPointless[]              = " You Asked Me To Compare Nothing, Which Was Pointless.";
static const char UnityStrNullPointerForExpected[] = " Expected pointer to be NULL";
static const char UnityStrNullPointerForActual[]   = " Actual pointer was NULL";

static const char UnityStrNot[]                    = "Not ";
static const char UnityStrInf[]                    = "Infinity";
static const char UnityStrNegInf[]                 = "Negative Infinity";
static const char UnityStrNaN[]                    = "NaN";
static const char UnityStrDet[]                    = "Determinate";
static const char UnityStrInvalidFloatTrait[]      = "Invalid Float Trait";

const char UnityStrErrShorthand[]                  = "Unity Shorthand Support Disabled";
const char UnityStrErrFloat[]                      = "Unity Floating Point Disabled";
const char UnityStrErrDouble[]                     = "Unity Double Precision Disabled";
const char UnityStrErr64[]                         = "Unity 64-bit Support Disabled";
const char UnityStrErrDetailStack[]                = "Unity Detail Stack Support Disabled";
static const char UnityStrBreaker[]                = "-----------------------";
static const char UnityStrResultsTests[]           = " Tests ";
static const char UnityStrResultsFailures[]        = " Failures ";
static const char UnityStrResultsIgnored[]         = " Ignored ";
# 78 "tests/unity/unity.c"
static const char UnityStrDetail1Name[]            = "Function"  " ";
static const char UnityStrDetail2Name[]            = " " "Argument"  " ";
# 88 "tests/unity/unity.c"
static void UnityPrintChar(const char* pch)
{

    if ((*pch <= 126) && (*pch >= 32))
    {
        (void)fputc((*pch),(&_iob[1]) ) ;
    }

    else if (*pch == 13)
    {
        (void)fputc(('\\'),(&_iob[1]) ) ;
        (void)fputc(('r'),(&_iob[1]) ) ;
    }

    else if (*pch == 10)
    {
        (void)fputc(('\\'),(&_iob[1]) ) ;
        (void)fputc(('n'),(&_iob[1]) ) ;
    }

    else
    {
        (void)fputc(('\\'),(&_iob[1]) ) ;
        (void)fputc(('x'),(&_iob[1]) ) ;
        UnityPrintNumberHex((UNITY_UINT)*pch, 2);
    }
}
# 138 "tests/unity/unity.c"
void UnityPrint(const char* string)
{
    const char* pch = string;

    if (pch != ((void *)0) )
    {
        while (*pch)
        {
# 154 "tests/unity/unity.c"
            UnityPrintChar(pch);
            pch++;
        }
    }
}

void UnityPrintLen(const char* string, const UNITY_UINT32 length)
{
    const char* pch = string;

    if (pch != ((void *)0) )
    {
        while (*pch && ((UNITY_UINT32)(pch - string) < length))
        {

            if ((*pch <= 126) && (*pch >= 32))
            {
                (void)fputc((*pch),(&_iob[1]) ) ;
            }

            else if (*pch == 13)
            {
                (void)fputc(('\\'),(&_iob[1]) ) ;
                (void)fputc(('r'),(&_iob[1]) ) ;
            }

            else if (*pch == 10)
            {
                (void)fputc(('\\'),(&_iob[1]) ) ;
                (void)fputc(('n'),(&_iob[1]) ) ;
            }

            else
            {
                (void)fputc(('\\'),(&_iob[1]) ) ;
                (void)fputc(('x'),(&_iob[1]) ) ;
                UnityPrintNumberHex((UNITY_UINT)*pch, 2);
            }
            pch++;
        }
    }
}


void UnityPrintIntNumberByStyle(const UNITY_INT number, const UNITY_DISPLAY_STYLE_T style)
{
    if ((style & (0x10) ) == (0x10) )
    {
        if (style == UNITY_DISPLAY_STYLE_CHAR)
        {

            (void)fputc(('\''),(&_iob[1]) ) ;
            if ((number <= 126) && (number >= 32))
            {
                (void)fputc(((int)number),(&_iob[1]) ) ;
            }

            else if (number == 13)
            {
                (void)fputc(('\\'),(&_iob[1]) ) ;
                (void)fputc(('r'),(&_iob[1]) ) ;
            }

            else if (number == 10)
            {
                (void)fputc(('\\'),(&_iob[1]) ) ;
                (void)fputc(('n'),(&_iob[1]) ) ;
            }

            else
            {
                (void)fputc(('\\'),(&_iob[1]) ) ;
                (void)fputc(('x'),(&_iob[1]) ) ;
                UnityPrintNumberHex((UNITY_UINT)number, 2);
            }
            (void)fputc(('\''),(&_iob[1]) ) ;
        }
        else
        {
            UnityPrintNumber(number);
        }
    }
}

void UnityPrintUintNumberByStyle(const UNITY_UINT number, const UNITY_DISPLAY_STYLE_T style)
{
    if ((style & (0x20) ) == (0x20) )
    {
        UnityPrintNumberUnsigned(number);
    }
    else
    {
        (void)fputc(('0'),(&_iob[1]) ) ;
        (void)fputc(('x'),(&_iob[1]) ) ;
        UnityPrintNumberHex((UNITY_UINT)number, (char)((style & 0xF) * 2));
    }
}


void UnityPrintNumber(const UNITY_INT number_to_print)
{
    UNITY_UINT number = (UNITY_UINT)number_to_print;

    if (number_to_print < 0)
    {

        (void)fputc(('-'),(&_iob[1]) ) ;
        number = (~number) + 1;
    }
    UnityPrintNumberUnsigned(number);
}



void UnityPrintNumberUnsigned(const UNITY_UINT number)
{
    UNITY_UINT divisor = 1;


    while (number / divisor > 9)
    {
        divisor *= 10;
    }


    do
    {
        (void)fputc(((char)('0' + (number / divisor % 10))),(&_iob[1]) ) ;
        divisor /= 10;
    } while (divisor > 0);
}


void UnityPrintNumberHex(const UNITY_UINT number, const char nibbles_to_print)
{
    int nibble;
    char nibbles = nibbles_to_print;

    if ((unsigned)nibbles > (8) )
    {
        nibbles = (8) ;
    }

    while (nibbles > 0)
    {
        nibbles--;
        nibble = (int)(number >> (nibbles * 4)) & 0x0F;
        if (nibble <= 9)
        {
            (void)fputc(((char)('0' + nibble)),(&_iob[1]) ) ;
        }
        else
        {
            (void)fputc(((char)('A' - 10 + nibble)),(&_iob[1]) ) ;
        }
    }
}


void UnityPrintMask(const UNITY_UINT mask, const UNITY_UINT number)
{
    UNITY_UINT current_bit = (UNITY_UINT)1 << (16  - 1);
    UNITY_INT32 i;

    for (i = 0; i < 16 ; i++)
    {
        if (current_bit & mask)
        {
            if (current_bit & number)
            {
                (void)fputc(('1'),(&_iob[1]) ) ;
            }
            else
            {
                (void)fputc(('0'),(&_iob[1]) ) ;
            }
        }
        else
        {
            (void)fputc(('X'),(&_iob[1]) ) ;
        }
        current_bit = current_bit >> 1;
    }
}
# 348 "tests/unity/unity.c"
void UnityPrintFloat(const UNITY_DOUBLE input_number)
{





    static const int sig_digits = 7;
    static const UNITY_INT32 min_scaled = 1000000;
    static const UNITY_INT32 max_scaled = 10000000;


    UNITY_DOUBLE number = input_number;


    if (number < 0.0f)
    {
        (void)fputc(('-'),(&_iob[1]) ) ;
        number = -number;
    }


    if (number == 0.0f)
    {
        UnityPrint("0");
    }
    else if (((number != number) ? 1 : 0) )
    {
        UnityPrint("nan");
    }
    else if (((((number) - (number) != (number) - (number)) ? 1 : 0)  && !((number != number) ? 1 : 0) ) )
    {
        UnityPrint("inf");
    }
    else
    {
        UNITY_INT32 n_int = 0;
        UNITY_INT32 n;
        int         exponent = 0;
        int         decimals;
        int         digits;
        char        buf[16] = {0};
# 398 "tests/unity/unity.c"
        if (number < 1.0f)
        {
            UNITY_DOUBLE factor = 1.0f;

            while (number < (UNITY_DOUBLE)max_scaled / 1e10f)  { number *= 1e10f; exponent -= 10; }
            while (number * factor < (UNITY_DOUBLE)min_scaled) { factor *= 10.0f; exponent--; }

            number *= factor;
        }
        else if (number > (UNITY_DOUBLE)max_scaled)
        {
            UNITY_DOUBLE divisor = 1.0f;

            while (number > (UNITY_DOUBLE)min_scaled * 1e10f)   { number  /= 1e10f; exponent += 10; }
            while (number / divisor > (UNITY_DOUBLE)max_scaled) { divisor *= 10.0f; exponent++; }

            number /= divisor;
        }
        else
        {





            UNITY_DOUBLE factor = 1.0f;
            n_int = (UNITY_INT32)number;
            number -= (UNITY_DOUBLE)n_int;

            while (n_int < min_scaled) { n_int *= 10; factor *= 10.0f; exponent--; }

            number *= factor;
        }


        n = ((UNITY_INT32)(number + number) + 1) / 2;



        if ((n & 1) && (((UNITY_DOUBLE)n - number) == 0.5f))
            n--;


        n += n_int;

        if (n >= max_scaled)
        {
            n = min_scaled;
            exponent++;
        }


        decimals = ((exponent <= 0) && (exponent >= -(sig_digits + 3))) ? (-exponent) : (sig_digits - 1);
        exponent += decimals;


        while ((decimals > 0) && ((n % 10) == 0))
        {
            n /= 10;
            decimals--;
        }


        digits = 0;
        while ((n != 0) || (digits <= decimals))
        {
            buf[digits++] = (char)('0' + n % 10);
            n /= 10;
        }


        while (digits > 0)
        {
            if (digits == decimals)
            {
                (void)fputc(('.'),(&_iob[1]) ) ;
            }
            (void)fputc((buf[--digits]),(&_iob[1]) ) ;
        }


        if (exponent != 0)
        {
            (void)fputc(('e'),(&_iob[1]) ) ;

            if (exponent < 0)
            {
                (void)fputc(('-'),(&_iob[1]) ) ;
                exponent = -exponent;
            }
            else
            {
                (void)fputc(('+'),(&_iob[1]) ) ;
            }

            digits = 0;
            while ((exponent != 0) || (digits < 2))
            {
                buf[digits++] = (char)('0' + exponent % 10);
                exponent /= 10;
            }
            while (digits > 0)
            {
                (void)fputc((buf[--digits]),(&_iob[1]) ) ;
            }
        }
    }
}



static void UnityTestResultsBegin(const char* file, const UNITY_UINT  line)
{
# 540 "tests/unity/unity.c"
    UnityPrint(file);
    (void)fputc((':'),(&_iob[1]) ) ;
    UnityPrintNumber((UNITY_INT)line);
    (void)fputc((':'),(&_iob[1]) ) ;
    UnityPrint(Unity.CurrentTestName);
    (void)fputc((':'),(&_iob[1]) ) ;



}


static void UnityTestResultsFailBegin(const UNITY_UINT  line)
{
    UnityTestResultsBegin(Unity.TestFile, line);
    UnityPrint(UnityStrFail);
    (void)fputc((':'),(&_iob[1]) ) ;
}


void UnityConcludeTest(void)
{
    if (Unity.CurrentTestIgnored)
    {
        Unity.TestIgnores++;
    }
    else if (!Unity.CurrentTestFailed)
    {
        UnityTestResultsBegin(Unity.TestFile, Unity.CurrentTestLineNumber);
        UnityPrint(UnityStrPass);
    }
    else
    {
        Unity.TestFailures++;
    }

    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;
    do {   } while (0) ;
    (void)fputc(('\n'),(&_iob[1]) ) ;
    (void)0 ;
}


static void UnityAddMsgIfSpecified(const char* msg)
{
# 617 "tests/unity/unity.c"
    if (Unity.CurrentDetail1)
    {
        UnityPrint(UnityStrSpacer);
        UnityPrint(UnityStrDetail1Name);
        UnityPrint(Unity.CurrentDetail1);
        if (Unity.CurrentDetail2)
        {
            UnityPrint(UnityStrDetail2Name);
            UnityPrint(Unity.CurrentDetail2);
        }
    }


    if (msg)
    {
        UnityPrint(UnityStrSpacer);
        UnityPrint(msg);
    }
}


static void UnityPrintExpectedAndActualStrings(const char* expected, const char* actual)
{
    UnityPrint(UnityStrExpected);
    if (expected != ((void *)0) )
    {
        (void)fputc(('\''),(&_iob[1]) ) ;
        UnityPrint(expected);
        (void)fputc(('\''),(&_iob[1]) ) ;
    }
    else
    {
        UnityPrint(UnityStrNull);
    }
    UnityPrint(UnityStrWas);
    if (actual != ((void *)0) )
    {
        (void)fputc(('\''),(&_iob[1]) ) ;
        UnityPrint(actual);
        (void)fputc(('\''),(&_iob[1]) ) ;
    }
    else
    {
        UnityPrint(UnityStrNull);
    }
}


static void UnityPrintExpectedAndActualStringsLen(const char* expected,
                                                  const char* actual,
                                                  const UNITY_UINT32 length)
{
    UnityPrint(UnityStrExpected);
    if (expected != ((void *)0) )
    {
        (void)fputc(('\''),(&_iob[1]) ) ;
        UnityPrintLen(expected, length);
        (void)fputc(('\''),(&_iob[1]) ) ;
    }
    else
    {
        UnityPrint(UnityStrNull);
    }
    UnityPrint(UnityStrWas);
    if (actual != ((void *)0) )
    {
        (void)fputc(('\''),(&_iob[1]) ) ;
        UnityPrintLen(actual, length);
        (void)fputc(('\''),(&_iob[1]) ) ;
    }
    else
    {
        UnityPrint(UnityStrNull);
    }
}
# 698 "tests/unity/unity.c"
static int UnityIsOneArrayNull( const void*  expected,
 const void*  actual,
                               const UNITY_UINT  lineNumber,
                               const char* msg)
{

    if (expected == actual) { return 0; }


    if (expected == ((void *)0) )
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrNullPointerForExpected);
        UnityAddMsgIfSpecified(msg);
        return 1;
    }


    if (actual == ((void *)0) )
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrNullPointerForActual);
        UnityAddMsgIfSpecified(msg);
        return 1;
    }

    return 0;
}
# 732 "tests/unity/unity.c"
void UnityAssertBits(const UNITY_INT mask,
                     const UNITY_INT expected,
                     const UNITY_INT actual,
                     const char* msg,
                     const UNITY_UINT  lineNumber)
{
    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if ((mask & expected) != (mask & actual))
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        UnityPrintMask((UNITY_UINT)mask, (UNITY_UINT)expected);
        UnityPrint(UnityStrWas);
        UnityPrintMask((UNITY_UINT)mask, (UNITY_UINT)actual);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}


void UnityAssertEqualIntNumber(const UNITY_INT expected,
                               const UNITY_INT actual,
                               const char* msg,
                               const UNITY_UINT  lineNumber,
                               const UNITY_DISPLAY_STYLE_T style)
{
    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if (expected != actual)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        UnityPrintIntNumberByStyle(expected, style);
        UnityPrint(UnityStrWas);
        UnityPrintIntNumberByStyle(actual, style);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}

void UnityAssertEqualUintNumber(const UNITY_UINT expected,
                                const UNITY_UINT actual,
                                const char* msg,
                                const UNITY_UINT  lineNumber,
                                const UNITY_DISPLAY_STYLE_T style)
{
    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if (expected != actual)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        UnityPrintUintNumberByStyle(expected, style);
        UnityPrint(UnityStrWas);
        UnityPrintUintNumberByStyle(actual, style);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}

void UnityAssertIntGreaterOrLessOrEqualNumber(const UNITY_INT threshold,
                                              const UNITY_INT actual,
                                              const UNITY_COMPARISON_T compare,
                                              const char *msg,
                                              const UNITY_UINT  lineNumber,
                                              const UNITY_DISPLAY_STYLE_T style)
{
    int failed = 0;
    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if ((threshold == actual) && !(compare & UNITY_EQUAL_TO)) { failed = 1; }

    if ((actual > threshold) && (compare & UNITY_SMALLER_THAN)) { failed = 1; }
    if ((actual < threshold) && (compare & UNITY_GREATER_THAN)) { failed = 1; }

    if (failed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        UnityPrintIntNumberByStyle(actual, style);
        if (compare & UNITY_GREATER_THAN) { UnityPrint(UnityStrGt);       }
        if (compare & UNITY_SMALLER_THAN) { UnityPrint(UnityStrLt);       }
        if (compare & UNITY_EQUAL_TO)     { UnityPrint(UnityStrOrEqual);  }
        if (compare == UNITY_NOT_EQUAL)   { UnityPrint(UnityStrNotEqual); }
        UnityPrintIntNumberByStyle(threshold, style);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}

void UnityAssertUintGreaterOrLessOrEqualNumber(const UNITY_UINT threshold,
                                               const UNITY_UINT actual,
                                               const UNITY_COMPARISON_T compare,
                                               const char *msg,
                                               const UNITY_UINT  lineNumber,
                                               const UNITY_DISPLAY_STYLE_T style)
{
    int failed = 0;
    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if ((threshold == actual) && !(compare & UNITY_EQUAL_TO)) { failed = 1; }


    if ((actual > threshold) && (compare & UNITY_SMALLER_THAN)) { failed = 1; }
    if ((actual < threshold) && (compare & UNITY_GREATER_THAN)) { failed = 1; }

    if (failed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        UnityPrintUintNumberByStyle(actual, style);
        if (compare & UNITY_GREATER_THAN) { UnityPrint(UnityStrGt);       }
        if (compare & UNITY_SMALLER_THAN) { UnityPrint(UnityStrLt);       }
        if (compare & UNITY_EQUAL_TO)     { UnityPrint(UnityStrOrEqual);  }
        if (compare == UNITY_NOT_EQUAL)   { UnityPrint(UnityStrNotEqual); }
        UnityPrintUintNumberByStyle(threshold, style);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}
# 863 "tests/unity/unity.c"
void UnityAssertEqualIntArray( const void*  expected,
 const void*  actual,
                              const UNITY_UINT32 num_elements,
                              const char* msg,
                              const UNITY_UINT  lineNumber,
                              const UNITY_DISPLAY_STYLE_T style,
                              const UNITY_FLAGS_T flags)
{
    UNITY_UINT32 elements  = num_elements;
    unsigned int length    = style & 0xF;
    unsigned int increment = 0;

    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if (num_elements == 0)
    {



        do {                                           UnityTestResultsFailBegin(lineNumber);     UnityPrint(UnityStrPointless);             UnityAddMsgIfSpecified(msg);               do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;                   } while (0) ;

    }

    if (expected == actual)
    {
        return;
    }

    if (UnityIsOneArrayNull(expected, actual, lineNumber, msg))
    {
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }

    while ((elements > 0) && (elements--))
    {
        UNITY_INT expect_val;
        UNITY_INT actual_val;

        switch (length)
        {
            case 1:
                expect_val = *( const UNITY_INT8*)expected;
                actual_val = *( const UNITY_INT8*)actual;
                if (style & ((0x20)  | (0x40) ))
                {
                    expect_val &= 0x000000FF;
                    actual_val &= 0x000000FF;
                }
                increment  = sizeof(UNITY_INT8);
                break;

            case 2:
                expect_val = *( const UNITY_INT16*)expected;
                actual_val = *( const UNITY_INT16*)actual;
                if (style & ((0x20)  | (0x40) ))
                {
                    expect_val &= 0x0000FFFF;
                    actual_val &= 0x0000FFFF;
                }
                increment  = sizeof(UNITY_INT16);
                break;
# 933 "tests/unity/unity.c"
            default:
            case 4:
                expect_val = *( const UNITY_INT32*)expected;
                actual_val = *( const UNITY_INT32*)actual;
# 944 "tests/unity/unity.c"
                increment  = sizeof(UNITY_INT32);
                length = 4;
                break;
        }

        if (expect_val != actual_val)
        {
            if ((style & (0x20) ) && (length < (16  / 8)))
            {
                UNITY_INT mask = 1;
                mask = (mask << 8 * length) - 1;
                expect_val &= mask;
                actual_val &= mask;
            }
            UnityTestResultsFailBegin(lineNumber);
            UnityPrint(UnityStrElement);
            UnityPrintNumberUnsigned(num_elements - elements - 1);
            UnityPrint(UnityStrExpected);
            UnityPrintIntNumberByStyle(expect_val, style);
            UnityPrint(UnityStrWas);
            UnityPrintIntNumberByStyle(actual_val, style);
            UnityAddMsgIfSpecified(msg);
            do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
        }

        if (flags == UNITY_ARRAY_TO_ARRAY)
        {
            expected = ( const void* )((const char*)expected + increment);
        }
        actual = ( const void* )((const char*)actual + increment);
    }
}
# 1008 "tests/unity/unity.c"
static int UnityFloatsWithin(UNITY_FLOAT delta, UNITY_FLOAT expected, UNITY_FLOAT actual)
{
    UNITY_FLOAT diff;
    if (((((expected) - (expected) != (expected) - (expected)) ? 1 : 0)  && !((expected != expected) ? 1 : 0) )  && ((((actual) - (actual) != (actual) - (actual)) ? 1 : 0)  && !((actual != actual) ? 1 : 0) )  && (((expected) < 0) == ((actual) < 0))) return 1;       if (((expected != expected) ? 1 : 0)  && ((actual != actual) ? 1 : 0) ) return 1;                                                                (diff) = (actual) - (expected);                                                               if ((diff) < 0) (diff) = -(diff);                                                             if ((delta) < 0) (delta) = -(delta);                                                          return !(((diff != diff) ? 1 : 0)  || ((((diff) - (diff) != (diff) - (diff)) ? 1 : 0)  && !((diff != diff) ? 1 : 0) )  || ((diff) > (delta))) ;
}


void UnityAssertWithinFloatArray(const UNITY_FLOAT delta,
 const UNITY_FLOAT* expected,
 const UNITY_FLOAT* actual,
                                 const UNITY_UINT32 num_elements,
                                 const char* msg,
                                 const UNITY_UINT  lineNumber,
                                 const UNITY_FLAGS_T flags)
{
    UNITY_UINT32 elements = num_elements;
 const UNITY_FLOAT* ptr_expected = expected;
 const UNITY_FLOAT* ptr_actual = actual;
    UNITY_FLOAT in_delta = delta;
    UNITY_FLOAT current_element_delta = delta;

    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if (elements == 0)
    {



        do {                                           UnityTestResultsFailBegin(lineNumber);     UnityPrint(UnityStrPointless);             UnityAddMsgIfSpecified(msg);               do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;                   } while (0) ;

    }

    if (((((in_delta) - (in_delta) != (in_delta) - (in_delta)) ? 1 : 0)  && !((in_delta != in_delta) ? 1 : 0) ) )
    {
        return;
    }

    if (((in_delta != in_delta) ? 1 : 0) )
    {

        do {                                           UnityTestResultsFailBegin(lineNumber);     UnityPrint(UnityStrPointless);             UnityAddMsgIfSpecified(msg);               do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;                   } while (0) ;
    }

    if (expected == actual)
    {
        return;
    }

    if (UnityIsOneArrayNull(( const void* )expected, ( const void* )actual, lineNumber, msg))
    {
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }


    if (in_delta < 0)
    {
        in_delta = -in_delta;
    }

    while (elements--)
    {
        current_element_delta = *ptr_expected * (0.00001f) ;

        if (current_element_delta < 0)
        {

            current_element_delta = -current_element_delta;
        }

        if (!UnityFloatsWithin(in_delta + current_element_delta, *ptr_expected, *ptr_actual))
        {
            UnityTestResultsFailBegin(lineNumber);
            UnityPrint(UnityStrElement);
            UnityPrintNumberUnsigned(num_elements - elements - 1);
            do {                                                                UnityPrint(UnityStrExpected);                                     UnityPrintFloat((UNITY_DOUBLE)*ptr_expected);                                        UnityPrint(UnityStrWas);                                          UnityPrintFloat((UNITY_DOUBLE)*ptr_actual);                                        } while (0) ;
            UnityAddMsgIfSpecified(msg);
            do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
        }
        if (flags == UNITY_ARRAY_TO_ARRAY)
        {
            ptr_expected++;
        }
        ptr_actual++;
    }
}


void UnityAssertFloatsWithin(const UNITY_FLOAT delta,
                             const UNITY_FLOAT expected,
                             const UNITY_FLOAT actual,
                             const char* msg,
                             const UNITY_UINT  lineNumber)
{
    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;


    if (!UnityFloatsWithin(delta, expected, actual))
    {
        UnityTestResultsFailBegin(lineNumber);
        do {                                                                UnityPrint(UnityStrExpected);                                     UnityPrintFloat((UNITY_DOUBLE)expected);                                        UnityPrint(UnityStrWas);                                          UnityPrintFloat((UNITY_DOUBLE)actual);                                        } while (0) ;
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}



void UnityAssertFloatsNotWithin(const UNITY_FLOAT delta,
                                const UNITY_FLOAT expected,
                                const UNITY_FLOAT actual,
                                const char* msg,
                                const UNITY_UINT  lineNumber)
{
    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if (UnityFloatsWithin(delta, expected, actual))
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        UnityPrintFloat((UNITY_DOUBLE)expected);
        UnityPrint(UnityStrNotEqual);
        UnityPrintFloat((UNITY_DOUBLE)actual);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}


void UnityAssertGreaterOrLessFloat(const UNITY_FLOAT threshold,
                                   const UNITY_FLOAT actual,
                                   const UNITY_COMPARISON_T compare,
                                   const char* msg,
                                   const UNITY_UINT  lineNumber)
{
    int failed;

    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    failed = 0;


    if (!(actual < threshold) && (compare & UNITY_SMALLER_THAN)) { failed = 1; }
    if (!(actual > threshold) && (compare & UNITY_GREATER_THAN)) { failed = 1; }

    if ((compare & UNITY_EQUAL_TO) && UnityFloatsWithin(threshold * (0.00001f) , threshold, actual)) { failed = 0; }

    if (failed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        UnityPrintFloat(actual);
        if (compare & UNITY_GREATER_THAN) { UnityPrint(UnityStrGt); }
        if (compare & UNITY_SMALLER_THAN) { UnityPrint(UnityStrLt); }
        if (compare & UNITY_EQUAL_TO)     { UnityPrint(UnityStrOrEqual);  }
        UnityPrintFloat(threshold);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}



void UnityAssertFloatSpecial(const UNITY_FLOAT actual,
                             const char* msg,
                             const UNITY_UINT  lineNumber,
                             const UNITY_FLOAT_TRAIT_T style)
{
    const char* trait_names[] = {UnityStrInf, UnityStrNegInf, UnityStrNaN, UnityStrDet};
    UNITY_INT should_be_trait = ((UNITY_INT)style & 1);
    UNITY_INT is_trait        = !should_be_trait;
    UNITY_INT trait_index     = (UNITY_INT)(style >> 1);

    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    switch (style)
    {
        case UNITY_FLOAT_IS_INF:
        case UNITY_FLOAT_IS_NOT_INF:
            is_trait = ((((actual) - (actual) != (actual) - (actual)) ? 1 : 0)  && !((actual != actual) ? 1 : 0) )  && (actual > 0);
            break;
        case UNITY_FLOAT_IS_NEG_INF:
        case UNITY_FLOAT_IS_NOT_NEG_INF:
            is_trait = ((((actual) - (actual) != (actual) - (actual)) ? 1 : 0)  && !((actual != actual) ? 1 : 0) )  && (actual < 0);
            break;

        case UNITY_FLOAT_IS_NAN:
        case UNITY_FLOAT_IS_NOT_NAN:
            is_trait = ((actual != actual) ? 1 : 0)  ? 1 : 0;
            break;

        case UNITY_FLOAT_IS_DET:
        case UNITY_FLOAT_IS_NOT_DET:
            is_trait = !((((actual) - (actual) != (actual) - (actual)) ? 1 : 0)  && !((actual != actual) ? 1 : 0) )  && !((actual != actual) ? 1 : 0) ;
            break;

        case UNITY_FLOAT_INVALID_TRAIT:
        default:
            trait_index = 0;
            trait_names[0] = UnityStrInvalidFloatTrait;
            break;
    }

    if (is_trait != should_be_trait)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrExpected);
        if (!should_be_trait)
        {
            UnityPrint(UnityStrNot);
        }
        UnityPrint(trait_names[trait_index]);
        UnityPrint(UnityStrWas);

        UnityPrintFloat((UNITY_DOUBLE)actual);
# 1229 "tests/unity/unity.c"
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}
# 1466 "tests/unity/unity.c"
void UnityAssertIntNumbersWithin(const UNITY_UINT delta,
                                 const UNITY_INT expected,
                                 const UNITY_INT actual,
                                 const char* msg,
                                 const UNITY_UINT  lineNumber,
                                 const UNITY_DISPLAY_STYLE_T style)
{
    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if (actual > expected)
    {
        Unity.CurrentTestFailed = (((UNITY_UINT)actual - (UNITY_UINT)expected) > delta);
    }
    else
    {
        Unity.CurrentTestFailed = (((UNITY_UINT)expected - (UNITY_UINT)actual) > delta);
    }

    if (Unity.CurrentTestFailed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrDelta);
        UnityPrintIntNumberByStyle((UNITY_INT)delta, style);
        UnityPrint(UnityStrExpected);
        UnityPrintIntNumberByStyle(expected, style);
        UnityPrint(UnityStrWas);
        UnityPrintIntNumberByStyle(actual, style);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}

void UnityAssertUintNumbersWithin(const UNITY_UINT delta,
                                  const UNITY_UINT expected,
                                  const UNITY_UINT actual,
                                  const char* msg,
                                  const UNITY_UINT  lineNumber,
                                  const UNITY_DISPLAY_STYLE_T style)
{
    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if (actual > expected)
    {
        Unity.CurrentTestFailed = ((actual - expected) > delta);
    }
    else
    {
        Unity.CurrentTestFailed = ((expected - actual) > delta);
    }

    if (Unity.CurrentTestFailed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrint(UnityStrDelta);
        UnityPrintUintNumberByStyle(delta, style);
        UnityPrint(UnityStrExpected);
        UnityPrintUintNumberByStyle(expected, style);
        UnityPrint(UnityStrWas);
        UnityPrintUintNumberByStyle(actual, style);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}


void UnityAssertNumbersArrayWithin(const UNITY_UINT delta,
 const void*  expected,
 const void*  actual,
                                   const UNITY_UINT32 num_elements,
                                   const char* msg,
                                   const UNITY_UINT  lineNumber,
                                   const UNITY_DISPLAY_STYLE_T style,
                                   const UNITY_FLAGS_T flags)
{
    UNITY_UINT32 elements = num_elements;
    unsigned int length   = style & 0xF;
    unsigned int increment = 0;

    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if (num_elements == 0)
    {



        do {                                           UnityTestResultsFailBegin(lineNumber);     UnityPrint(UnityStrPointless);             UnityAddMsgIfSpecified(msg);               do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;                   } while (0) ;

    }

    if (expected == actual)
    {
        return;
    }

    if (UnityIsOneArrayNull(expected, actual, lineNumber, msg))
    {
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }

    while ((elements > 0) && (elements--))
    {
        UNITY_INT expect_val;
        UNITY_INT actual_val;

        switch (length)
        {
            case 1:

                if ((style & (0x10) ) == (0x10) )
                {
                    expect_val = *( const UNITY_INT8*)expected;
                    actual_val = *( const UNITY_INT8*)actual;
                    increment  = sizeof(UNITY_INT8);
                }
                else
                {
                    expect_val = (UNITY_INT)*( const UNITY_UINT8*)expected;
                    actual_val = (UNITY_INT)*( const UNITY_UINT8*)actual;
                    increment  = sizeof(UNITY_UINT8);
                }
                break;

            case 2:

                if ((style & (0x10) ) == (0x10) )
                {
                    expect_val = *( const UNITY_INT16*)expected;
                    actual_val = *( const UNITY_INT16*)actual;
                    increment  = sizeof(UNITY_INT16);
                }
                else
                {
                    expect_val = (UNITY_INT)*( const UNITY_UINT16*)expected;
                    actual_val = (UNITY_INT)*( const UNITY_UINT16*)actual;
                    increment  = sizeof(UNITY_UINT16);
                }
                break;
# 1622 "tests/unity/unity.c"
            default:
            case 4:

                if ((style & (0x10) ) == (0x10) )
                {
                    expect_val = *( const UNITY_INT32*)expected;
                    actual_val = *( const UNITY_INT32*)actual;
                    increment  = sizeof(UNITY_INT32);
                }
                else
                {
                    expect_val = (UNITY_INT)*( const UNITY_UINT32*)expected;
                    actual_val = (UNITY_INT)*( const UNITY_UINT32*)actual;
                    increment  = sizeof(UNITY_UINT32);
                }
                length = 4;
                break;
        }

        if ((style & (0x10) ) == (0x10) )
        {
            if (actual_val > expect_val)
            {
                Unity.CurrentTestFailed = (((UNITY_UINT)actual_val - (UNITY_UINT)expect_val) > delta);
            }
            else
            {
                Unity.CurrentTestFailed = (((UNITY_UINT)expect_val - (UNITY_UINT)actual_val) > delta);
            }
        }
        else
        {
            if ((UNITY_UINT)actual_val > (UNITY_UINT)expect_val)
            {
                Unity.CurrentTestFailed = (((UNITY_UINT)actual_val - (UNITY_UINT)expect_val) > delta);
            }
            else
            {
                Unity.CurrentTestFailed = (((UNITY_UINT)expect_val - (UNITY_UINT)actual_val) > delta);
            }
        }

        if (Unity.CurrentTestFailed)
        {
            if ((style & (0x20) ) && (length < (16  / 8)))
            {
                UNITY_INT mask = 1;
                mask = (mask << 8 * length) - 1;
                expect_val &= mask;
                actual_val &= mask;
            }
            UnityTestResultsFailBegin(lineNumber);
            UnityPrint(UnityStrDelta);
            UnityPrintIntNumberByStyle((UNITY_INT)delta, style);
            UnityPrint(UnityStrElement);
            UnityPrintNumberUnsigned(num_elements - elements - 1);
            UnityPrint(UnityStrExpected);
            UnityPrintIntNumberByStyle(expect_val, style);
            UnityPrint(UnityStrWas);
            UnityPrintIntNumberByStyle(actual_val, style);
            UnityAddMsgIfSpecified(msg);
            do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
        }

        if (flags == UNITY_ARRAY_TO_ARRAY)
        {
            expected = ( const void* )((const char*)expected + increment);
        }
        actual = ( const void* )((const char*)actual + increment);
    }
}


void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_UINT  lineNumber)
{
    UNITY_UINT32 i;

    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;


    if (expected && actual)
    {
        for (i = 0; expected[i] || actual[i]; i++)
        {
            if (expected[i] != actual[i])
            {
                Unity.CurrentTestFailed = 1;
                break;
            }
        }
    }
    else
    {
        if (expected || actual)
        {
            Unity.CurrentTestFailed = 1;
        }
    }

    if (Unity.CurrentTestFailed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrintExpectedAndActualStrings(expected, actual);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}


void UnityAssertEqualStringLen(const char* expected,
                               const char* actual,
                               const UNITY_UINT32 length,
                               const char* msg,
                               const UNITY_UINT  lineNumber)
{
    UNITY_UINT32 i;

    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;


    if (expected && actual)
    {
        for (i = 0; (i < length) && (expected[i] || actual[i]); i++)
        {
            if (expected[i] != actual[i])
            {
                Unity.CurrentTestFailed = 1;
                break;
            }
        }
    }
    else
    {
        if (expected || actual)
        {
            Unity.CurrentTestFailed = 1;
        }
    }

    if (Unity.CurrentTestFailed)
    {
        UnityTestResultsFailBegin(lineNumber);
        UnityPrintExpectedAndActualStringsLen(expected, actual, length);
        UnityAddMsgIfSpecified(msg);
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }
}


void UnityAssertEqualStringArray( const void*  expected,
                                 const char** actual,
                                 const UNITY_UINT32 num_elements,
                                 const char* msg,
                                 const UNITY_UINT  lineNumber,
                                 const UNITY_FLAGS_T flags)
{
    UNITY_UINT32 i = 0;
    UNITY_UINT32 j = 0;
    const char* expd = ((void *)0) ;
    const char* act = ((void *)0) ;

    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;


    if (num_elements == 0)
    {



        do {                                           UnityTestResultsFailBegin(lineNumber);     UnityPrint(UnityStrPointless);             UnityAddMsgIfSpecified(msg);               do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;                   } while (0) ;

    }

    if ((const void*)expected == (const void*)actual)
    {
        return;
    }

    if (UnityIsOneArrayNull(( const void* )expected, ( const void* )actual, lineNumber, msg))
    {
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }

    if (flags != UNITY_ARRAY_TO_ARRAY)
    {
        expd = (const char*)expected;
    }

    do
    {
        act = actual[j];
        if (flags == UNITY_ARRAY_TO_ARRAY)
        {
            expd = ((const char* const*)expected)[j];
        }


        if (expd && act)
        {
            for (i = 0; expd[i] || act[i]; i++)
            {
                if (expd[i] != act[i])
                {
                    Unity.CurrentTestFailed = 1;
                    break;
                }
            }
        }
        else
        {
            if (expd != act)
            {
                Unity.CurrentTestFailed = 1;
            }
        }

        if (Unity.CurrentTestFailed)
        {
            UnityTestResultsFailBegin(lineNumber);
            if (num_elements > 1)
            {
                UnityPrint(UnityStrElement);
                UnityPrintNumberUnsigned(j);
            }
            UnityPrintExpectedAndActualStrings(expd, act);
            UnityAddMsgIfSpecified(msg);
            do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
        }
    } while (++j < num_elements);
}


void UnityAssertEqualMemory( const void*  expected,
 const void*  actual,
                            const UNITY_UINT32 length,
                            const UNITY_UINT32 num_elements,
                            const char* msg,
                            const UNITY_UINT  lineNumber,
                            const UNITY_FLAGS_T flags)
{
 const unsigned char* ptr_exp = ( const unsigned char*)expected;
 const unsigned char* ptr_act = ( const unsigned char*)actual;
    UNITY_UINT32 elements = num_elements;
    UNITY_UINT32 bytes;

    do { if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) { longjmp(Unity.AbortFrame, 1) ; } } while (0) ;

    if (elements == 0)
    {



        do {                                           UnityTestResultsFailBegin(lineNumber);     UnityPrint(UnityStrPointless);             UnityAddMsgIfSpecified(msg);               do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;                   } while (0) ;

    }
    if (length == 0)
    {
        do {                                           UnityTestResultsFailBegin(lineNumber);     UnityPrint(UnityStrPointless);             UnityAddMsgIfSpecified(msg);               do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;                   } while (0) ;
    }

    if (expected == actual)
    {
        return;
    }

    if (UnityIsOneArrayNull(expected, actual, lineNumber, msg))
    {
        do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
    }

    while (elements--)
    {
        bytes = length;
        while (bytes--)
        {
            if (*ptr_exp != *ptr_act)
            {
                UnityTestResultsFailBegin(lineNumber);
                UnityPrint(UnityStrMemory);
                if (num_elements > 1)
                {
                    UnityPrint(UnityStrElement);
                    UnityPrintNumberUnsigned(num_elements - elements - 1);
                }
                UnityPrint(UnityStrByte);
                UnityPrintNumberUnsigned(length - bytes - 1);
                UnityPrint(UnityStrExpected);
                UnityPrintIntNumberByStyle(*ptr_exp, UNITY_DISPLAY_STYLE_HEX8);
                UnityPrint(UnityStrWas);
                UnityPrintIntNumberByStyle(*ptr_act, UNITY_DISPLAY_STYLE_HEX8);
                UnityAddMsgIfSpecified(msg);
                do { Unity.CurrentTestFailed  = 1; (void)0 ; longjmp(Unity.AbortFrame, 1) ; } while (0) ;
            }
            ptr_exp++;
            ptr_act++;
        }
        if (flags == UNITY_ARRAY_TO_VAL)
        {
            ptr_exp = ( const unsigned char*)expected;
        }
    }
}



static union
{
    UNITY_INT8 i8;
    UNITY_INT16 i16;
    UNITY_INT32 i32;




    float f;




} UnityQuickCompare;

 const void*  UnityNumToPtr(const UNITY_INT num, const UNITY_UINT8 size)
{
    switch(size)
    {
        case 1:
            UnityQuickCompare.i8 = (UNITY_INT8)num;
            return ( const void* )(&UnityQuickCompare.i8);

        case 2:
            UnityQuickCompare.i16 = (UNITY_INT16)num;
            return ( const void* )(&UnityQuickCompare.i16);
# 1964 "tests/unity/unity.c"
        default:
            UnityQuickCompare.i32 = (UNITY_INT32)num;
            return ( const void* )(&UnityQuickCompare.i32);
    }
}



 const void*  UnityFloatToPtr(const float num)
{
  