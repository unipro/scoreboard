#ifndef __GAMECONFIG_HPP__
#define __GAMECONFIG_HPP__

class GameConfig
{
public:
    GameConfig();

    void SetPeriodLengthInMin(int min) {
        m_periLen = min * 60 * 1000;
    }

    int GetPeriodLengthInMin() const {
        return m_periLen / (60 * 1000);
    }

    void SetPeriodLength(int millis) {
        m_periLen = millis;
    }

    int GetPeriodLength() const {
        return m_periLen;
    }

    void SetShotClockInSec(int sec) {
        m_shotClock = sec * 1000;
    }

    int GetShotClockInSec() const {
        return m_shotClock / 1000;
    }

    void SetShotClock(int millis) {
        m_shotClock = millis;
    }

    int GetShotClock() const {
        return m_shotClock;
    }

    void SetBuzzerLength(int millis) {
        m_buzLen = millis;
    }

    int GetBuzzerLength() const {
        return m_buzLen;
    }

private:
    int m_periLen;
    int m_shotClock;
    int m_buzLen;
};

#endif  // __GAMECONFIG_HPP__
