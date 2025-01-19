#include <gui/game_screen/GameView.hpp>
#include "main.h"
#include <time.h>
#include <stdint.h>

static uint32_t seed = 123456789; // Initial seed, can be set dynamically

void srand_custom(uint32_t new_seed) {
    seed = new_seed;
}

uint32_t rand_custom(void) {
    seed = (1103515245 * seed + 12345) & 0x7FFFFFFF; // LCG formula
    return seed;
}

unsigned int checksum(uint8_t *buf, int n) {
   unsigned int sum = 0;
   while (n--) {
      sum += *(buf++);
   }
   return sum;
}

#define BUFFER_SIZE 14

typedef struct {
    uint16_t ballX;
    uint8_t ballY;
    uint8_t boxY;
    int8_t boxSpeedY;
    uint8_t redScore;
    uint8_t blueScore;
    uint8_t pause;
    uint8_t replay;
    uint8_t resetPos;
    uint8_t boardSpeed;
    uint8_t win;
    uint8_t checkSum;
} GameState;

typedef union {
    GameState state;
    uint8_t buffer[BUFFER_SIZE];
} GameStateUnion;

extern uint32_t blueRight, blueLeft;
extern uint32_t redRight, redLeft;
extern UART_HandleTypeDef huart2;
//extern uint8_t *txBuffer, *rxBuffer;
extern int uartSent;
extern int ballSpeed, boardSpeed, scoreLimit;

extern GameStateUnion txBuffer, rxBuffer, buf, stateUnion;

GameView::GameView()
{

}

void GameView::setupScreen()
{
	GameViewBase::setupScreen();
	speedX = ballSpeed;
//	srand(time(NULL));
	speedX = rand_custom()%2 ? speedX : -speedX;
	speedY = rand_custom()%4;

}

void GameView::tearDownScreen()
{
    GameViewBase::tearDownScreen();
}

void GameView::tickEvent()
{
	encodeState(1);
	memcpy(txBuffer.buffer,stateUnion.buffer,BUFFER_SIZE*sizeof(uint8_t));
	if(uartSent){
		uartSent = 0;
		HAL_UART_Transmit_DMA(&huart2, (const uint8_t*)txBuffer.buffer, BUFFER_SIZE);
	}

	memcpy(stateUnion.buffer,rxBuffer.buffer,BUFFER_SIZE*sizeof(uint8_t));
	if(stateUnion.state.checkSum == checksum(stateUnion.buffer, 12)){
		decodeState(1);
	}

	if(countDownBell){
		countDownBell--;
	}else{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	}

	if(pauseBlue || pauseRed)return;

	if(replayBlue || replayRed){
		replayGame();
		replayBlue = 0;
	}

	if(resetPos){
		resetPos = 0;
	}

	if(wait){
		HAL_Delay(wait*1000);
		wait = 0;
	}

	tickCountX = (tickCountX + (520 + speedX))%520;
	tickCountY = (tickCountY + (400 + speedY))%400;

	ballX = ((tickCountX > 260) ? (tickCountX - 260) : (260 - tickCountX)) + 20;
	ballY = ((tickCountY > 200) ? (tickCountY - 200) : (200 - tickCountY)) + 10;

	ball.moveTo(ballX, ballY);
	ball.invalidate();

	blueSpeedY = blueRight - blueLeft;
	int newBlueY = blueY + boardSpeed*blueSpeedY;
	blueY = (newBlueY <= 0) ? 0 : (newBlueY >= 190) ? 190 : (uint16_t) newBlueY;
//	blueY = ballY;
	blueBox.moveTo(10, blueY);
	blueBox.invalidate();

//	redSpeedY = redRight - redLeft;
//	int newRedY = redY + boardSpeed*redSpeedY;
//	redY = (newRedY <= 0) ? 0 : (newRedY >= 190) ? 190 : (uint16_t) newRedY;
//	redY = ballY;
	redBox.moveTo(300, redY);
	redBox.invalidate();

	if(ballX < 20 + ballSpeed){
		if(ballY + 20 + boardSpeed < blueY || ballY > blueY + 50 + boardSpeed){
			updateScoreRed(1);
			speedY = rand_custom()%4;
			if(redScore >= scoreLimit){
				win = 1;
				endGame(0);
			}
		}else{
			speedX = rand_custom()%2 ? speedX : -speedX;
			speedY += (speedY >= 0)&&(tickCountY > 200) ? blueSpeedY : -blueSpeedY;
			speedY = (speedY <= -4) ? -4 : (speedY >= 4) ? 4 : speedY;
		}
	}else if(ballX > 280 - ballSpeed){
		if(ballY + 20 + boardSpeed < redY || ballY > redY + 50 + boardSpeed){
			updateScoreBlue(1);
			speedY = rand_custom()%4;
			if(blueScore >= scoreLimit){
				win = 2;
				endGame(1);
			}
		}else{
			speedX = rand_custom()%2 ? speedX : -speedX;
			speedY += (speedY >= 0)&&(tickCountY > 200) ? redSpeedY : -redSpeedY;
			speedY = (speedY <= -4) ? -4 : (speedY >= 4) ? 4 : speedY;
		}
	}
}

void GameView::updateScoreRed(int num)
{
	redScore += num;
	Unicode::snprintf(redTextBuffer, 10, "%03d", redScore);
    redText.setWildcard(redTextBuffer);
    redText.invalidate(); // Refresh the text area to display the new value
    resetPosition();
    resetPos = 1;
    if(num){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
		countDownBell = 10;
		wait = 1;
    }
}

void GameView::updateScoreBlue(int num)
{
	blueScore += num;
	Unicode::snprintf(blueTextBuffer, 10, "%03d", blueScore);
    blueText.setWildcard(blueTextBuffer);
    blueText.invalidate(); // Refresh the text area to display the new value
    resetPosition();
    resetPos = 1;
    if(num){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
		countDownBell = 10;
		wait = 1;
	}
}

void GameView::setScoreRed(int num)
{
	redScore = num;
	Unicode::snprintf(redTextBuffer, 10, "%03d", num);
    redText.setWildcard(redTextBuffer);
    redText.invalidate(); // Refresh the text area to display the new value
}

void GameView::setScoreBlue(int num)
{
	blueScore = num;
	Unicode::snprintf(blueTextBuffer, 10, "%03d", num);
    blueText.setWildcard(blueTextBuffer);
    blueText.invalidate(); // Refresh the text area to display the new value
}

void GameView::handlePlayButton()
{
	pauseBlue = 0;
}

void GameView::handlePauseButton()
{
	pauseBlue = 1;
}

void GameView::handleReplayButton()
{
	replayGame();
	replayBlue = 1;
	pauseBlue = 0;
}

void GameView::replayGame()
{
	setScoreBlue(0);
	setScoreRed(0);
	resetPosition();
	wait = 2;
}

void GameView::resetPosition()
{
	tickCountX = 150;
	tickCountY = 110;
	ballX = 150;
	ballY = 110;
	ball.moveTo(ballX, ballY);
	ball.invalidate();
	blueY = 95;
	blueBox.moveTo(10, blueY);
	blueBox.invalidate();
	redY = 95;
	redBox.moveTo(300, redY);
	redBox.invalidate();
}

void GameView::encodeState(int isBlue)
{
	if (isBlue) {
		stateUnion.state.ballX = ballX;
		stateUnion.state.ballY = ballY;
		stateUnion.state.boxY = blueY;
		stateUnion.state.boxSpeedY = blueSpeedY;
		stateUnion.state.redScore = redScore;
		stateUnion.state.blueScore = blueScore;
		stateUnion.state.pause = pauseBlue;
		stateUnion.state.replay = replayBlue;
		stateUnion.state.resetPos = resetPos;
		stateUnion.state.boardSpeed = boardSpeed;
		stateUnion.state.win = win;
	} else {
		stateUnion.state.boxY = redY;
		stateUnion.state.boxSpeedY = redSpeedY;
		stateUnion.state.pause = pauseRed;
		stateUnion.state.replay = replayRed;
	}
	stateUnion.state.checkSum = checksum(stateUnion.buffer, 12);
}

void GameView::decodeState(int isBlue)
{
	if (isBlue) {
		redY = stateUnion.state.boxY;
		redSpeedY = stateUnion.state.boxSpeedY;
		pauseRed = stateUnion.state.pause;
		replayRed = stateUnion.state.replay;
	} else {
		ballX = stateUnion.state.ballX;
		ballY = stateUnion.state.ballY;
		blueY = stateUnion.state.boxY;
		blueSpeedY = stateUnion.state.boxSpeedY;
		redScore = stateUnion.state.redScore;
		blueScore = stateUnion.state.blueScore;
		pauseBlue = stateUnion.state.pause;
		replayBlue = stateUnion.state.replay;
		resetPos = stateUnion.state.resetPos;
		boardSpeed = stateUnion.state.boardSpeed;
		win = stateUnion.state.win;
	}
}

void GameView::endGame(int isBlue){
	if(isBlue){
		blueWin.setVisible(true);
		blueWin.invalidate();
	}else{
		redWin.setVisible(true);
		redWin.invalidate();
	}

	backButton.setVisible(true);
	backButton.invalidate();

	pauseButton.setVisible(false);
	pauseButton.invalidate();

//	replayButton.setVisible(true);
//	replayButton.invalidate();

	pauseBlue = 1;
}
