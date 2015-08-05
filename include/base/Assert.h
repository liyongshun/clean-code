/*
 * Assert.h
 *
 *  Created on: 2014.5.24
 *      Author: liyongshun
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#include "base/Status.h"

#define ASSERT_TRUE(exp)   \
    do{                    \
        if(!(exp))         \
        {                  \
            return FAILED; \
        }                  \
    }while(0)

#define ASSERT_TRUE_BOOL(exp)   \
    do{                         \
        if(!(exp))              \
        {                       \
            return false;       \
        }                       \
    }while(0)

#define ASSERT_SUCC_CALL(call) \
    do{                        \
        Status status = call;  \
        if(status != SUCCESS)  \
        {                      \
            return status;     \
        }                      \
    }while(0)

#define ASSERT_VALID_PTR(exp)  \
    do{                        \
        if(exp == 0)        \
        {                      \
            return FAILED;     \
        }                      \
    }while(0)

#define ASSERT_VALID_PTR_BOOL(exp)  \
    do{                        \
        if(exp == 0)        \
        {                      \
            return FALSE;     \
        }                      \
    }while(0)

#endif /* ASSERT_H_ */
