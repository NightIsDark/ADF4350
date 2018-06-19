#include "key.h"
#ifdef KEY

#define KEY	 	((P0 & 0x80)?1:0)          //P0.7   key

unsigned char KeyCheck(void)
{
	return KEY;
}


#endif