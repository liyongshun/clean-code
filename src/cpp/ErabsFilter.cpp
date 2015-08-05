/*
 * ErabFilter.cpp
 *
 *  Created on: 2014.5.24
 *      Author: liyongshun
 */
#include "cpp/ErabsFilter.hpp"
#include "erab/ErabErrorCode.h"
#include "erab/ErabInfo.h"
#include "base/Assert.h"

namespace
{
    bool checkRange(BYTE data, BYTE lower, BYTE upper)
    {
        return data >= lower && data <= upper;
    }

    bool isErabIdValid(BYTE erabId)
    {
        return checkRange(erabId, 1, 15);
    }

    //////////////////////////////////////////////////////////
    bool isQciValid(BYTE qci)
    {
        return checkRange(qci, 1, 9);
    }

    bool isArpValid(const ArpIE& arp)
    {
        return checkRange(arp.level, 1, 15);
    }

    //////////////////////////////////////////////////////////////////
    bool isGbr(BYTE qci)
    {
        return checkRange(qci, 1, 4);
    }

    bool isGbrBitRateValid(const GbrIE& gbrIE)
    {
        return gbrIE.dlGbr <= gbrIE.dlMbr &&
               gbrIE.ulGbr <= gbrIE.ulMbr;
    }

    //////////////////////////////////////////////////////////////
    bool isGbrIEValid(const QosPara& qosPara)
    {
        if(!isGbr(qosPara.qci)) return true;

        if(qosPara.grbIEPresent == 0) return true;

        return isGbrBitRateValid(qosPara.gbrIE);
    }

    Status verifyQosPara(const QosPara& qos)
    {
        if(!isQciValid(qos.qci)) return E_INVALID_QCI;

        if(!isArpValid(qos.arp)) return E_INVALID_QOS_ARP;

        if(!isGbrIEValid(qos))   return E_INVALID_QOS_GBR;

        return SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////
    template<typename ERAB_LIST, typename ERAB_TYPE>
    struct Erabs
    {
        Erabs(ERAB_LIST& list) : list(list)
        {
        }

        bool contains(BYTE erabId) const
        {
            for(BYTE index = 0; index < list.num; ++index)
            {
                if(erabId == list.erabs[index].erabId) return true;
            }

            return false;
        }

        Status add(const ERAB_TYPE& erab)
        {
            ASSERT_TRUE(list.num < MAX_ERAB_NUM_PER_UE);
            ASSERT_TRUE(!contains(erab.erabId));

            list.erabs[list.num++] = erab;
            return SUCCESS;
        }

        void remove(const ERAB_TYPE& erab)
        {
            BYTE anchor = 0;
            for(BYTE index = 0; index < list.num; ++index)
            {
                if(erab.erabId == list.erabs[index].erabId)
                {
                    anchor = index;
                    break;
                }
            }

            for(BYTE index = anchor; index < list.num; ++index)
            {
               list.erabs[index] = list.erabs[index+1];
            }

            if(anchor < list.num) list.num--;
        }

    private:
        ERAB_LIST& list;
    };
}

#define SuccessErabs Erabs<SuccessErabList, Erab>(succList)
#define FailedErabs  Erabs<FailedErabList,  FailedErab>(failedList)
///////////////////////////////////////////////////////
Status ErabFilter::verifyErabId(BYTE erabId) const
{
    if(!isErabIdValid(erabId)) return E_INVALID_ERAB_ID;

    if( SuccessErabs.contains(erabId) ||
        FailedErabs.contains(erabId)) return E_DUP_ERAB_ID;

    return SUCCESS;
}

Status ErabFilter::verify(const Erab& erab) const
{
    ASSERT_SUCC_CALL(verifyErabId(erab.erabId));
    ASSERT_SUCC_CALL(verifyQosPara(erab.qosPara));

    return SUCCESS;
}

ErabFilter::ErabFilter(SuccessErabList& succList, FailedErabList& failedList) 
    : succList(succList), failedList(failedList)
{
}

Status ErabFilter::process(const Erab& erab)
{
    Status status = verify(erab);

    if(SUCCESS != status)
    {
        if(status == E_DUP_ERAB_ID) SuccessErabs.remove(erab);

        FailedErab failedErab = { erab.erabId, status };
        return FailedErabs.add(failedErab);
    }

    return SuccessErabs.add(erab);
}
