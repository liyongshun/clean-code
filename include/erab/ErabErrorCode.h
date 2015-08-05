/*
 * ErabErrorCode.h
 *
 *  Created on: 2014.5.24
 *      Author: liyongshun
 */

#ifndef ERABERRORCODE_H_
#define ERABERRORCODE_H_

#include "base/Status.h"

#define ERAB_ERROR_CODE_BEGIN 1000
#define ERAB_ERROR_CODE(status, value)\
        FAILED_STATUS_VALUE(status, (value + ERAB_ERROR_CODE_BEGIN))

ERAB_ERROR_CODE(E_INVALID_ERAB_ID, 0);
ERAB_ERROR_CODE(E_DUP_ERAB_ID,     1);
ERAB_ERROR_CODE(E_INVALID_QCI,     2);
ERAB_ERROR_CODE(E_INVALID_QOS_ARP, 3);
ERAB_ERROR_CODE(E_INVALID_QOS_GBR, 4);

#endif /* ERABERRORCODE_H_ */
