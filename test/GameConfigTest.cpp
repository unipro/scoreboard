#include "../CppUnitLite/TestHarness.h"
#include "../model/GameConfig.hpp"

TEST(DefaultGameConfig, GameConfig)
{
    GameConfig config;
    LONGS_EQUAL(20 * 60 * 1000, config.GetPeriodLength());
    LONGS_EQUAL(30 * 1000, config.GetShotClock());
}

TEST(SetPeriodLengthInMin, GameConfig)
{
    GameConfig config;
    config.SetPeriodLengthInMin(30);
    LONGS_EQUAL(30 * 60 * 1000, config.GetPeriodLength());
    LONGS_EQUAL(30, config.GetPeriodLengthInMin());
}

TEST(SetShotClockInSec, GameConfig)
{
    GameConfig config;
    config.SetShotClockInSec(14);
    LONGS_EQUAL(14 * 1000, config.GetShotClock());
    LONGS_EQUAL(14, config.GetShotClockInSec());
}
