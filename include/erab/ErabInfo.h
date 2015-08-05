/*
 * ErabInfo.h
 *
 *  Created on: 2014.5.24
 *      Author: liyongshun
 */

#ifndef ERABINFO_H_
#define ERABINFO_H_

#include "base/BaseType.h"

/*
ErabItem
    |-erabId
    |-qosPara
        |-qci
        |-arp
              |-level
             |-emptionCap
             |-emptionVulnerability
        |-gbrIE[option]
            |-ulMbr
            |-dlMbr
            |-ulGbr
            |-dlGbr
*/

#define MAX_ERAB_NUM_PER_UE  8

typedef struct ArpIE
{
    BYTE level;
    BYTE emptionCap;
    BYTE emptionVulnerability;
}ArpIE;

typedef struct GbrIE
{
    WORD64 ulMbr;
    WORD64 dlMbr;
    WORD64 ulGbr;
    WORD64 dlGbr;
}GbrIE;

typedef struct QosPara
{
    BYTE  grbIEPresent;
    BYTE  qci;
    ArpIE arp;
    GbrIE gbrIE;
}QosPara;

typedef struct Erab
{
    BYTE    erabId;
    QosPara qosPara;
}Erab;

////////////////////////////////////////////////////////////////////////////////////
typedef struct SuccessErabList
{
    BYTE num;
    Erab erabs[MAX_ERAB_NUM_PER_UE];
}SuccessErabList;

typedef struct FailedErab
{
    BYTE erabId;
    WORD32 cause;
}FailedErab;

typedef struct FailedErabList
{
    BYTE num;
    FailedErab erabs[MAX_ERAB_NUM_PER_UE];
}FailedErabList;

#endif /* ERABINFO_H_ */

