#include "../CppUnitLite/TestHarness.h"
#include "../model/GameTime.hpp"

#if defined(_MSC_VER)
#  include <windows.h>
#else
#  include <unistd.h>
void
Sleep(unsigned long millis)
{
    usleep(1000 * millis);
}
#endif

class GameTimeTest : public IGameTimeView, public IGameControlView
{
public:
    GameTimeTest() : m_gameClock(-1), m_shotClock(-1), m_isBuzzing(false) {
    }

    // IGameTimeView methods
    void UpdateGameClock(int rest) {
        m_gameClock = rest;
    }

    void UpdateShotClock(int rest) {
        m_shotClock = rest;
    }

    void UpdatePeriod(int n) {
        m_period = n;
    }

    void StartBuzzer() {
        m_isBuzzing = true;
    }

    void StopBuzzer() {
        m_isBuzzing = false;
    }

    // IGameControlView methods
    void EnablePlay(bool enable) {
        m_enablePlay = enable;
    }

    void EnablePause(bool enable) {
        m_enablePause = enable;
    }


    // GameTimeTest methods
    int GetGameClock() {
        return m_gameClock;
    }

    int GetShotClock() {
        return m_shotClock;
    }

    int GetPeriod() {
        return m_period;
    }

    bool IsBuzzing() {
        return m_isBuzzing;
    }

    bool IsPlayEnable() {
        return m_enablePlay;
    }

    bool IsPauseEnable() {
        return m_enablePause;
    }

private:
    int m_gameClock;
    int m_shotClock;
    int m_period;
    bool m_isBuzzing;
    bool m_enablePlay;
    bool m_enablePause;
};

TEST(InitGameTime, GameTimeTest)
{
    GameTimeTest view;
    GameConfig config;
    GameTime model(view, view, config);
    LONGS_EQUAL(config.GetPeriodLength(), view.GetGameClock());
    LONGS_EQUAL(config.GetShotClock(), view.GetShotClock());
    LONGS_EQUAL(0, view.GetPeriod());
    CHECK(!model.IsPlaying());
}

TEST(PlayGameTest, GameTimeTest)
{
    GameTimeTest view;
    GameConfig config;
    GameTime model(view, view, config);

    model.Play();
    CHECK(view.IsPauseEnable());
    CHECK(!view.IsPlayEnable());

    model.Pause();
    CHECK(!view.IsPauseEnable());
    CHECK(view.IsPlayEnable());
}

TEST(PeriodTest, GameTimeTest)
{
    GameTimeTest view;
    GameConfig config;
    GameTime model(view, view, config);

    model.IncrPeriod();
    LONGS_EQUAL(1, view.GetPeriod());

    model.IncrPeriod();
    LONGS_EQUAL(2, view.GetPeriod());

    model.IncrPeriod();
    LONGS_EQUAL(3, view.GetPeriod());

    model.DecrPeriod();
    LONGS_EQUAL(2, view.GetPeriod());

    model.DecrPeriod();
    LONGS_EQUAL(1, view.GetPeriod());

    model.DecrPeriod();
    LONGS_EQUAL(0, view.GetPeriod());

    for (int i = 0; i < 5; i++)
        model.IncrPeriod();
    model.ResetPeriod();
    LONGS_EQUAL(0, view.GetPeriod());
}

TEST(GameClockTest, GameTimeTest)
{
    GameTimeTest view;
    GameConfig config;
    GameTime model(view, view, config);
    int interval = 100;
    int i;

    int expGameClock = config.GetPeriodLength();
    int expShotClock = config.GetShotClock();
    for (i = 0; i < 10; i++) {
        Sleep(interval);

        model.TickTock();

        LONGS_EQUAL(expGameClock, view.GetGameClock());
        LONGS_EQUAL(expShotClock, view.GetShotClock());
    }

    long playStartTime = GetTickCount();
    long elapsedTime;
    model.Play();
    LONGS_EQUAL(expGameClock, view.GetGameClock());
    LONGS_EQUAL(expShotClock, view.GetShotClock());

    for (i = 0; i < 10; i++) {
        Sleep(interval);

        model.TickTock();
        elapsedTime = GetTickCount() - playStartTime;
        expGameClock = config.GetPeriodLength() - elapsedTime;
        expShotClock = config.GetShotClock() - elapsedTime;

        LONGS_EQUAL(expGameClock, view.GetGameClock());
        LONGS_EQUAL(expShotClock, view.GetShotClock());
    }

    model.Pause();

    long pauseStartTime = GetTickCount();
    for (i = 0; i < 10; i++) {
        Sleep(interval);

        model.TickTock();

        LONGS_EQUAL(expGameClock, view.GetGameClock());
        LONGS_EQUAL(expShotClock, view.GetShotClock());
    }

    model.Play();
    long pauseDuration = GetTickCount() - pauseStartTime;

    for (i = 0; i < 10; i++) {
        Sleep(interval);

        model.TickTock();
        elapsedTime = GetTickCount() - playStartTime - pauseDuration;
        expGameClock = config.GetPeriodLength() - elapsedTime;
        expShotClock = config.GetShotClock() - elapsedTime;

        LONGS_EQUAL(expGameClock, view.GetGameClock());
        LONGS_EQUAL(expShotClock, view.GetShotClock());
    }

    model.IncrGameClock();
    playStartTime += 1000;        // playTime = current - start
    elapsedTime = GetTickCount() - playStartTime - pauseDuration;
    expGameClock = config.GetPeriodLength() - elapsedTime;
    LONGS_EQUAL(expGameClock, view.GetGameClock());
    LONGS_EQUAL(expShotClock, view.GetShotClock());

    model.DecrGameClock();
    playStartTime -= 1000;        // playTime = current - start
    elapsedTime = GetTickCount() - playStartTime - pauseDuration;
    expGameClock = config.GetPeriodLength() - elapsedTime;
    LONGS_EQUAL(expGameClock, view.GetGameClock());
    LONGS_EQUAL(expShotClock, view.GetShotClock());

    model.ResetShotClock();
    long attackStartTime = GetTickCount();

    for (i = 0; i < 10; i++) {
        Sleep(interval);

        model.TickTock();
        elapsedTime = GetTickCount() - playStartTime - pauseDuration;
        expGameClock = config.GetPeriodLength() - elapsedTime;
        long attackElapsedTime = GetTickCount() - attackStartTime;
        expShotClock = config.GetShotClock() - attackElapsedTime;

        LONGS_EQUAL(expGameClock, view.GetGameClock());
        LONGS_EQUAL(expShotClock, view.GetShotClock());
    }
}

TEST(GameTimeExpireTest, GameTimeTest)
{
    GameTimeTest view;
    GameConfig config;
    config.SetPeriodLength(1000);
    config.SetBuzzerLength(500);
    GameTime model(view, view, config);
    int i;

    model.Play();
    for (i = 0; i < 100; i++) {
        Sleep(100);
        model.TickTock();
        if (view.GetGameClock() >= config.GetPeriodLength()) {
            break;
        }
    }

    CHECK(view.IsBuzzing());

    Sleep(config.GetBuzzerLength() * 2);
    model.TickTock();
    CHECK(! view.IsBuzzing());

    model.TickTock();
    CHECK(! view.IsBuzzing());
}

TEST(AttackTimeExpireTest, GameTimeTest)
{
    GameTimeTest view;
    GameConfig config;
    config.SetShotClock(1000);
    config.SetBuzzerLength(500);
    GameTime model(view, view, config);
    int i;

    model.Play();
    for (i = 0; i < 100; i++) {
        Sleep(100);
        model.TickTock();
        if (view.GetShotClock() == 0) {
            break;
        }
    }

    CHECK(view.IsBuzzing());

    Sleep(config.GetBuzzerLength() * 2);
    model.TickTock();
    CHECK(! view.IsBuzzing());

    model.TickTock();
    CHECK(! view.IsBuzzing());
}
