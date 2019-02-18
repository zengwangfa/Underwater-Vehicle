#ifndef _FSLTYPES_H_
#define _FSLTYPES_H_

#include "common.h"




#define FLAG_SET(BitNumber, Register)        (Register |=(1<<BitNumber))
#define FLAG_CLR(BitNumber, Register)        (Register &=~(1<<BitNumber))
#define FLAG_CHK(BitNumber, Register)        (Register & (1<<BitNumber))

#define _OUT    1
#define _IN     0
#define _ON      0
#define _OFF     1
#define _HIGH    1
#define _LOW     0



#define _BGND   asm(HALT)
#define _NOP    asm(NOP)



#endif /* _FSLTYPES_H_ */