/*
 * ErabsFilter.h
 *
 *  Created on: 2014.5.24
 *      Author: liyongshun
 */

#ifndef ERABSFILTER_H_
#define ERABSFILTER_H_

#include "base/Status.h"
#include "erab/ErabInfo.h"

#ifdef __cplusplus
extern "C" {
#endif

Status filterErabs(const Erab* erab, SuccessErabList* succList, FailedErabList* failList);

#ifdef __cplusplus
}
#endif

#endif /* ERABSFILTER_H_ */
