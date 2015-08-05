/*
 * ErabsFilter.c
 *
 *  Created on: 2014.5.24     
 *      Author: liyongshun
 */
#include "c/ErabsFilter.h"
#include "base/Assert.h"
#include "erab/ErabErrorCode.h"

static BOOLEAN isQciValid(BYTE qci)
{
    return qci >= 1 && qci <= 9;
}

static BOOLEAN isArpValid(const ArpIE* arp)
{
    ASSERT_VALID_PTR_BOOL(arp);

    return arp->level >= 1 && arp->level <= 15;
}

static BOOLEAN isGbr(BYTE qci)
{
    return qci >= 1 && qci <= 4;
}

static BOOLEAN isGbrBitRateValid(const GbrIE* gbrIE)
{
    ASSERT_VALID_PTR_BOOL(gbrIE);

    return gbrIE->dlGbr <= gbrIE->dlMbr &&
           gbrIE->ulGbr <= gbrIE->ulMbr;
}

static BOOLEAN isGbrIEValid(const QosPara* qosPara)
{
    if(!isGbr(qosPara->qci)) return TRUE;

    if(qosPara->grbIEPresent == 0) return TRUE;

    return isGbrBitRateValid(&qosPara->gbrIE);
}

static Status verifyQosPara(const QosPara* qos)
{
    if(!isQciValid(qos->qci)) return E_INVALID_QCI;

    if(!isArpValid(&qos->arp)) return E_INVALID_QOS_ARP;

    if(!isGbrIEValid(qos))   return E_INVALID_QOS_GBR;

    return SUCCESS;
}

/////////////////////////////////////////////////////////////////////////

static BOOLEAN isErabIdValid(const BYTE erabId)
{
    return erabId >= 1 && erabId <= 15;
}

static BOOLEAN containsInSuccList(BYTE erabId, SuccessErabList* succList)
{
    BYTE index;

    for(index = 0; index < succList->num; ++index)
    {
        if(succList->erabs[index].erabId == erabId) return TRUE;
    }

    return FALSE;
}

static BOOLEAN containsInFailList(BYTE erabId, FailedErabList* failList)
{
    BYTE index;

    for(index = 0; index < failList->num; ++index)
    {
        if(failList->erabs[index].erabId == erabId) return TRUE;
    }

    return FALSE;
}

static Status verifyErabId(BYTE erabId, SuccessErabList* succList, FailedErabList* failList)
{
    if(!isErabIdValid(erabId)) return E_INVALID_ERAB_ID;

    if( containsInSuccList(erabId, succList) ||
        containsInFailList(erabId, failList)) return E_DUP_ERAB_ID;

    return SUCCESS;
}

static Status verify(const Erab* erab, SuccessErabList* succList, FailedErabList* failList)
{
    ASSERT_SUCC_CALL(verifyErabId(erab->erabId, succList, failList));
    ASSERT_SUCC_CALL(verifyQosPara(&erab->qosPara));

    return SUCCESS;
}

static Status addToSuccessErabList(const Erab* erab, SuccessErabList* succList)
{
    ASSERT_TRUE(succList->num < MAX_ERAB_NUM_PER_UE);
    ASSERT_TRUE(!containsInSuccList(erab->erabId, succList));

    succList->erabs[succList->num++] = *erab;

    return SUCCESS;
}

static void removeFromSuccessErabList(const Erab* erab, SuccessErabList* succList)
{
    if( ! containsInSuccList(erab->erabId, succList)) return;

    BYTE index = 0;
    BYTE anchor = 0;
    for(index = 0; index < succList->num; ++index)
    {
       if(erab->erabId == succList->erabs[index].erabId)
       {
           anchor = index;
           break;
       }
    }

    for(index = anchor; index < succList->num; ++index)
    {
        succList->erabs[index] = succList->erabs[index+1];
    }

    if(anchor < succList->num) succList->num--;
}

static Status addToFailedErabList(const FailedErab* erab, FailedErabList* failList)
{
    ASSERT_TRUE(failList->num < MAX_ERAB_NUM_PER_UE);

    ASSERT_TRUE(!containsInFailList(erab->erabId, failList));

    failList->erabs[failList->num++] = *erab;

    return SUCCESS;
}

Status filterErabs(const Erab* erab, SuccessErabList* succList, FailedErabList* failList)
{
    Status status;

    ASSERT_VALID_PTR(erab);
    ASSERT_VALID_PTR(succList);
    ASSERT_VALID_PTR(failList);

    status = verify(erab, succList, failList);

    if(status != SUCCESS)
    {
        if(status == E_DUP_ERAB_ID) removeFromSuccessErabList(erab, succList);

        FailedErab failedErab = {erab->erabId, status};
        return addToFailedErabList(&failedErab, failList);
    }

    return addToSuccessErabList(erab, succList);
}


