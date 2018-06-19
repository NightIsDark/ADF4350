#ifndef AD4530_H

#define AD4530_H

#include "ADF4350_IO.h"
#include "compiler_defs.h"
#include "C8051F300_defs.h"

extern U8 set_pll_reg(U32 freq);
extern void SetPllTo1000(void);

#endif
