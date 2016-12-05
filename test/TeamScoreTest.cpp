#include "../CppUnitLite/TestHarness.h"
#include "../model/TeamScore.hpp"

class TeamScoreTest : public ITeamScoreView
{
public:
    TeamScoreTest() : m_score(-1), m_foul(-1) {
    }

    // ITeamScoreView methods
    void UpdateScore(int n) {
        m_score = n;
    }
    void UpdateFoul(int n) {
        m_foul = n;
    }

    // TeamScoreTest methods
    int GetScore() {
        return m_score;
    }
    int GetFoul() {
        return m_foul;
    }

private:
    int m_score;
    int m_foul;
};

TEST(InitTeamScore, TeamScoreTest)
{
    TeamScoreTest view;
    TeamScore model(view);
    LONGS_EQUAL(0, view.GetScore());
    LONGS_EQUAL(0, view.GetFoul());
}

TEST(TestScore, TeamScoreTest)
{
    TeamScoreTest view;
    TeamScore model(view);

    model.IncrScore();
    LONGS_EQUAL(1, view.GetScore());
    LONGS_EQUAL(0, view.GetFoul());

    model.IncrScore();
    LONGS_EQUAL(2, view.GetScore());
    LONGS_EQUAL(0, view.GetFoul());

    model.IncrScore();
    LONGS_EQUAL(3, view.GetScore());
    LONGS_EQUAL(0, view.GetFoul());

    model.DecrScore();
    LONGS_EQUAL(2, view.GetScore());
    LONGS_EQUAL(0, view.GetFoul());

    model.DecrScore();
    LONGS_EQUAL(1, view.GetScore());
    LONGS_EQUAL(0, view.GetFoul());

    model.DecrScore();
    LONGS_EQUAL(0, view.GetScore());
    LONGS_EQUAL(0, view.GetFoul());

    for (int i = 0; i < 10; i++) {
        model.IncrScore();
	}
    model.ResetScore();
    LONGS_EQUAL(0, view.GetScore());
}

TEST(TestFoul, TeamScoreTest)
{
    TeamScoreTest view;
    TeamScore model(view);

    model.IncrFoul();
    LONGS_EQUAL(0, view.GetScore());
    LONGS_EQUAL(1, view.GetFoul());

    model.IncrFoul();
    LONGS_EQUAL(0, view.GetScore());
    LONGS_EQUAL(2, view.GetFoul());

    model.IncrFoul();
    LONGS_EQUAL(0, view.GetScore());
    LONGS_EQUAL(3, view.GetFoul());

    model.DecrFoul();
    LONGS_EQUAL(0, view.GetScore());
    LONGS_EQUAL(2, view.GetFoul());

    model.DecrFoul();
    LONGS_EQUAL(0, view.GetScore());
    LONGS_EQUAL(1, view.GetFoul());

    model.DecrFoul();
    LONGS_EQUAL(0, view.GetScore());
    LONGS_EQUAL(0, view.GetFoul());

    for (int i = 0; i < 10; i++) {
        model.IncrFoul();
	}
    model.ResetFoul();
    LONGS_EQUAL(0, view.GetFoul());
}
