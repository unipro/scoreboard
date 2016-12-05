#ifndef __TEAMSCORE_HPP__
#define __TEAMSCORE_HPP__

#include "Interfaces.hpp"

class TeamScore
{
public:
    TeamScore(ITeamScoreView& view);

    void Reset();

    void ResetScore();
    void IncrScore();
    void DecrScore();

    void ResetFoul();
    void IncrFoul();
    void DecrFoul();

private:
    ITeamScoreView& m_view;
    int m_score;
    int m_foul;
};

#endif  // __TEAMSCORE_HPP__
