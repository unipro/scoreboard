#include "GameConfig.hpp"

static const long defaultPeriodLength = 20 * 60 * 1000; // 20 min
static const long defaultShotClock    = 30 * 1000;      // 30 sec
static const long defaultBuzzerTime   = 1000;           // 1 sec

GameConfig::GameConfig()
    : m_periLen(defaultPeriodLength)
    , m_shotClock(defaultShotClock)
    , m_buzLen(defaultBuzzerTime)
{
}
