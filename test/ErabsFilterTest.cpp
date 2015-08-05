/*
 * SuccessErabListTest.cpp
 *
 *  Created on: 2014.5.25
 *      Author: liyongshun
 */
#include "gtest/gtest.h"
#include "erab/ErabInfo.h"
#include "erab/ErabErrorCode.h"
#include "cpp/ErabsFilter.hpp"
#include "c/ErabsFilter.h"

//#define ERABS_FILTER_CPP

namespace
{
    const BYTE GIVEN_ERAB_ID = 1;
    const BYTE GIVEN_INVALID_ERAB_ID = 16;
    const BYTE GIVEN_QCI = 1;
    const BYTE GIVEN_INVALID_QCI = 10;

    const BYTE GIVEN_ARP_LEVEL = 15;
    const BYTE GIVEN_INVALID_ARP_LEVEL = 16;

    const WORD64 GIVEN_UL_MBR = 10000;
    const WORD64 GIVEN_DL_MBR = 20000;
    const WORD64 GIVEN_UL_GBR = 10000;
    const WORD64 GIVEN_DL_GBR = 20000;

    const WORD64 GIVEN_INVALID_UL_GBR = GIVEN_UL_MBR + 1;
    const WORD64 GIVEN_INVALID_DL_GBR = GIVEN_DL_MBR + 1;
}

struct ErabsFilterTest : testing::Test
{
#ifdef ERABS_FILTER_CPP
    ErabsFilterTest() : erabFilter(succList, failList)
    {
    }
#else
    ErabsFilterTest()
    {
    }
#endif

    Status filter(const Erab& erab)
    {
#ifdef ERABS_FILTER_CPP
        return erabFilter.process(erab);
#else
        return filterErabs(&erab, &succList, &failList);
#endif
    }

    virtual void SetUp()
    {
        init();
    }

    virtual void init()
    {
        initErab();
        initErabList();
    }

    void initErabList()
    {
        succList.num = 0;
        failList.num = 0;
    }

    void initErab()
    {
        erab.erabId = GIVEN_ERAB_ID;

        erab.qosPara.qci = GIVEN_QCI;
        erab.qosPara.arp.level = GIVEN_ARP_LEVEL;

        erab.qosPara.grbIEPresent = PRESENT;
        erab.qosPara.gbrIE.ulMbr = GIVEN_UL_MBR;
        erab.qosPara.gbrIE.dlMbr = GIVEN_DL_MBR;

        erab.qosPara.gbrIE.ulGbr = GIVEN_UL_GBR;
        erab.qosPara.gbrIE.dlGbr = GIVEN_DL_GBR;
    }

    void buildErabByInvalidErabId()
    {
        erab.erabId = GIVEN_INVALID_ERAB_ID;
    }

    void buildErabByInvalidQci()
    {
        erab.qosPara.qci = GIVEN_INVALID_QCI;
    }

    void buildErabByInvalidArpLevel()
    {
        erab.qosPara.arp.level = GIVEN_INVALID_ARP_LEVEL;
    }

    void buildErabByInvalidUlGbr()
    {
        erab.qosPara.gbrIE.ulGbr = GIVEN_INVALID_UL_GBR;
    }

    void buildErabByInvalidDlGbr()
    {
        erab.qosPara.gbrIE.dlGbr = GIVEN_INVALID_DL_GBR;
    }

    void assertErabs(Status cause, BYTE erabId = GIVEN_ERAB_ID)
    {
        filter(erab);

        ASSERT_EQ(failList.num, 1);
        ASSERT_EQ(failList.erabs[0].erabId, erabId);
        ASSERT_EQ(failList.erabs[0].cause, cause);

        ASSERT_EQ(succList.num, 0);
    }

protected:
    Erab erab;
    SuccessErabList succList;
    FailedErabList failList;

#ifdef ERABS_FILTER_CPP
    ErabFilter erabFilter;
#endif

};

TEST_F(ErabsFilterTest, should_add_to_SuccessErabList_given_a_valid_erab)
{
    ASSERT_EQ(succList.num, 0);

    filter(erab);

    ASSERT_EQ(succList.num, 1);
    ASSERT_EQ(succList.erabs[0].erabId, GIVEN_ERAB_ID);
}

TEST_F(ErabsFilterTest, should_add_to_FailedErabList_given_a_erab_of_invalid_erabId)
{
    buildErabByInvalidErabId();
    assertErabs(E_INVALID_ERAB_ID, GIVEN_INVALID_ERAB_ID);
}

TEST_F(ErabsFilterTest, should_add_to_FailedErabList_given_a_erab_of_invalid_qci)
{
    buildErabByInvalidQci();
    assertErabs(E_INVALID_QCI);
}

TEST_F(ErabsFilterTest, should_add_to_FailedErabList_given_a_erab_of_invalid_arp_level)
{
    buildErabByInvalidArpLevel();
    assertErabs(E_INVALID_QOS_ARP);
}

TEST_F(ErabsFilterTest, should_add_to_FailedErabList_given_a_erab_of_invalid_ul_gbr_para)
{
    buildErabByInvalidUlGbr();
    assertErabs(E_INVALID_QOS_GBR);
}

TEST_F(ErabsFilterTest, should_add_to_FailedErabList_given_a_erab_of_invalid_dl_gbr_para)
{
    buildErabByInvalidDlGbr();
    assertErabs(E_INVALID_QOS_GBR);
}

TEST_F(ErabsFilterTest, should_add_to_FailedErabList_when_process_the_same_erab_twice)
{
    ASSERT_EQ(failList.num, 0);

    filter(erab);
    filter(erab);

    ASSERT_EQ(failList.num, 1);
    ASSERT_EQ(failList.erabs[0].erabId, GIVEN_ERAB_ID);
    ASSERT_EQ(failList.erabs[0].cause, E_DUP_ERAB_ID);
}

TEST_F(ErabsFilterTest, should_add_to_FailedErabList_and_remove_from_SuccessErabList_when_process_the_same_erab_twice)
{
    filter(erab);
    ASSERT_EQ(succList.num, 1);
    ASSERT_EQ(succList.erabs[0].erabId, GIVEN_ERAB_ID);

    filter(erab);
    ASSERT_EQ(failList.num, 1);
    ASSERT_EQ(succList.num, 0);
}
