#ifndef STATUS_H_
#define STATUS_H_

#include "base/BaseType.h"

typedef unsigned long Status;

#define FAILED_STATUS_BEGIN 0x80000000

#define __FAILED_STATUS_VALUE(value) (FAILED_STATUS_BEGIN | (value))
#define FAILED_STATUS_VALUE(status, value)\
    const Status status = __FAILED_STATUS_VALUE(value);

const Status SUCCESS = 0;
FAILED_STATUS_VALUE(FAILED, 0);

#define PRESENT (BYTE)1
#define ABSENT (BYTE)0
#define TRUE (BOOLEAN)1
#define FALSE (BOOLEAN)0

#endif /* STATUS_H_ */
