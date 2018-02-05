# scoreboard
다양한 데스크톱 운영체제에서 동작하는 GUI 기반의 운동 경기 점수 전광판 프로그램

# 라이선스
점수판은 GNU GPL v3로 배포되는 오픈소스입니다.

# 사용법
* scoreboard(윈도우즈에서는 scoreboard.exe)를 실행시키고, 중앙 하단의 관리 패널의 버튼으로 조작합니다.
* 몇몇 단축키를 지원합니다.
  * 스페이스키 - 경기 시간을 시작/중단시킵니다.
  * 리턴키 - 공격 시간을 리셋합니다.
  * CTRL + 왼쪽 커서키 - 왼쪽의 점수를 하나 증가시킨다.
  * CTRL + 오른쪽 커서키 - 오른쪽의 점수를 하나 증가시킨다.
  * ALT + 왼쪽 커서키 - 왼쪽의 팀파울을 하나 증가시킨다.
  * ALT + 오른쪽 커서키 - 오른쪽의 팀파울을 하나 증가시킨다.

# 빌드방법

## 빌드에 필요한 라이브러리
* [wxWidgets](http://www.wxwidgets.org) - 그래픽 인터페이스 엔진
* [SDL](http://www.libsdl.org) - 오디오 출력

## 운영체제와 개발 도구에 따른 개발 환경 설정
* 윈도우 Visual Studio 2008
  * wxWidgets: http://www.rhyous.com/2009/12/16/how-to-compile-a-wxwidgets-application-in-visual-studio-2008/
    * WXWIN3 환경 변수에 wxWidgets SDK 설치 경로 설정, 예를 들면
      `WXWIN3=C:\wxMSW-3.0.0`
  * SDL: http://www.libsdl.org/download-2.0.php 에서 Development Libraries > Visual C++ 다운로드
    * SDL2 환경 변수에 SDL SDK 설치 경로 설정, 예를 들면
      `SDL2=C:\SDL-2.0.1`
* Debian 계열
  * wxWidgets
    `sudo apt-get install libwxgtk3.0-dev`
  * SDL
    `sudo apt-get install libsdl2-dev`
* Redhat 계열
  * wxWidgets
    `sudo yum install wxGTK-devel wxBase`
  * SDL
    `sudo yum install SDL-devel`

## 운영체제와 개발 도구에 따른 빌드와 설치
* Linux, Mac OSX, MinGW
  1. 소스의 최상의 디렉토리에서 ./configure 실행
  1. make 실행
  1. make install
* Visual Studio 9.0
  1. Visual Studio로 vstudio9.0/scoreboard.sln를 열고, 메뉴의 Build->"Build Solution" 클릭

# 관련정보 링크
* [홈페이지](http://blog.naver.com/scoreboard)
* [www.pcscoreboards.com](http://www.pcscoreboards.com)
  * 현재로써는 점수판보다 더 기능적으로 뛰어난 프로그램입니다.
  * 그러나 오픈소스가 아니고 윈도우가 아닌 다른 운영체제를 지원하지 않습니다.
* [농구 규칙](http://www.wkbl.or.kr/about/manage_system.asp)
