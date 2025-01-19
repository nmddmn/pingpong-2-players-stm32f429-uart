#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include <gui_generated/game_screen/GameViewBase.hpp>
#include <gui/game_screen/GamePresenter.hpp>

class GameView : public GameViewBase
{
public:
    GameView();
    virtual ~GameView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void tickEvent();
    void updateScoreBlue(int num);
    void updateScoreRed(int num);
    void setScoreBlue(int num);
    void setScoreRed(int num);

    void handlePlayButton();
    void handlePauseButton();
    void handleReplayButton();

    void replayGame();
    void resetPosition();

    void encodeState(int isBlue);
    void decodeState(int isBlue);

    void endGame(int isBlue);
protected:
    uint16_t tickCountX = 150;
    uint16_t tickCountY = 110;
    int speedX = 2;
    int speedY = 2;
    uint16_t ballX = 150;
    uint16_t ballY = 110;

    int8_t blueSpeedY = 0;
    int8_t redSpeedY = 0;

    uint16_t blueY = 95;
    uint16_t redY = 95;

    int blueScore = 0;
    int redScore = 0;

    int pauseBlue = 1;
    int pauseRed = 1;

    int replayBlue = 0;
    int replayRed = 0;

    int wait = 1;
    int countDownBell = 0;

    int resetPos = 0;

    int win = 0;
};

#endif // GAMEVIEW_HPP
