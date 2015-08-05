/*
 * ErabFilter.h
 *
 *  Created on: 2014.5.24
 *      Author: liyongshun
 */

#ifndef ERABFILTER_HPP_
#define ERABFILTER_HPP_

#include "base/Status.h"

struct SuccessErabList;
struct FailedErabList; 
struct Erab;

struct ErabFilter
{
    ErabFilter(SuccessErabList&, FailedErabList&);
    Status process(const Erab&);

private:
    Status verify(const Erab& erab) const;
    Status verifyErabId(BYTE erabId) const;

private:
    SuccessErabList& succList;
    FailedErabList& failedList;
};

#endif /* ERABFILTER_H_ */
