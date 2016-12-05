#include <iostream>
#include "main.hpp"
#include "GameConfigDatabase.hpp"
#include "ScoreboardFrame.hpp"

IMPLEMENT_APP(MyApp)

bool
MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    // 영구 저장소에서 프로그램의 설정을 읽어온다.
    GameConfigDatabase db;
    db.Read(g_config);

    // 프로그램의 메임프레임 시작
    ScoreboardFrame *boardFrame = new ScoreboardFrame(wxT("ScoreBoard"));
    boardFrame->Centre();
    boardFrame->SetMinSize(ScoreboardFrame::MIN_SIZE);
    boardFrame->Show(true);

    return true;
}

int
MyApp::OnRun()
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << '\n';
        return -1;
    }

    // start the main loop
    return wxApp::OnRun();    
}

int
MyApp::OnExit()
{
    // cleanup SDL
    SDL_Quit();

    // 영구 저장소에 프로그램의 설정을 기록한다.
    GameConfigDatabase db;
    db.Write(g_config);
    
    // return the standard exit code
    return wxApp::OnExit();
}
