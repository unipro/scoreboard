#ifndef __GAMETIME_HPP__
#define __GAMETIME_HPP__

#include "Interfaces.hpp"
#include "GameConfig.hpp"

#if !defined(_MSC_VER)
unsigned long GetTickCount();
#endif

class StopWatchTimer
{
public:
    StopWatchTimer();

    void Start();
    void Pause();
    void Stop();

    long GetTime() const;
    void SetTime(long millis);

    void IncrTime(long millis = 1000);
    void DecrTime(long millis = 1000);

private:
    enum { 
        Timer_Stopped,
        Timer_Paused,
        Timer_Running
    } m_state;
    long m_startTime;
    long m_pauseTime;
};

class GameTime
{
public:
    GameTime(IGameTimeView& view, IGameControlView& ctrlView, GameConfig& config);

    void TickTock();

    void ResetTime();

    void Play();
    void Pause();
    bool IsPlaying() const {
        return m_isPlaying;
    }

    int GetGameClock() const;
    void SetGameClock(int millis);
    void IncrGameClock();
    void DecrGameClock();

    void ResetShotClock();
    void IncrShotClock();
    void DecrShotClock();

    void IncrPeriod();
    void DecrPeriod();
    void ResetPeriod();

private:
    void UpdateTime();
    void UpdateView();
    void SoundBuzzer();
    void StopBuzzer();
    void PlayInterval();
    void PauseInterval();

private:
    GameConfig& m_config;
    IGameTimeView& m_view;
    IGameControlView& m_ctrlView;

    bool m_isPlaying;

    StopWatchTimer m_stopWatch;
    long m_playTime;
    long m_attackStartTime;
    long m_attackTime;

    enum {
        Buzzer_Ready,
        Buzzer_EndOfPeriod,
        Buzzer_EndOfAttack,
        Buzzer_Unknown
    } m_buzzerReason;
    bool m_isBuzzing;
    long m_buzzerStartTime;

    int m_period;
};

#endif  // __GAMETIME_HPP__
