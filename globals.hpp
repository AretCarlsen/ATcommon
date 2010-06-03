
/*  From: http://www.fefe.de/intof.html  */
#define __HALF_MAX_SIGNED(type) ((type)1 << (sizeof(type)*8-2))
#define __MAX_SIGNED(type) (__HALF_MAX_SIGNED(type) - 1 + __HALF_MAX_SIGNED(type))
//#define __MIN_SIGNED(type) (-1 - __MAX_SIGNED(type))
#define __MIN_SIGNED(type) (0 - __MAX_SIGNED(type))
#define __MIN(type) ((type)-1 < 1?__MIN_SIGNED(type):(type)0)
#define __MAX(type) ((type)~__MIN(type))

