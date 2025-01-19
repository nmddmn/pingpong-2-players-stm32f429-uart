#include <gui/settings_screen/SettingsView.hpp>

extern int ballSpeed, boardSpeed, scoreLimit;

SettingsView::SettingsView()
{

}

void SettingsView::setupScreen()
{
    SettingsViewBase::setupScreen();
    setBallSpeed(ballSpeed);
    setBoardSpeed(boardSpeed);
    setScoreLimit(scoreLimit);
}

void SettingsView::tearDownScreen()
{
    SettingsViewBase::tearDownScreen();
}

void SettingsView::ballSpeedIncreaseFunction(){
	ballSpeed++;
	ballSpeed = ballSpeed > 4 ? 4 : ballSpeed;
	setBallSpeed(ballSpeed);
}

void SettingsView::ballSpeedDecreaseFunction(){
	ballSpeed--;
	ballSpeed = ballSpeed < 1 ? 1 : ballSpeed;
	setBallSpeed(ballSpeed);
}

void SettingsView::boardSpeedIncreaseFunction(){
	boardSpeed++;
	boardSpeed = boardSpeed > 4 ? 4 : boardSpeed;
	setBoardSpeed(boardSpeed);
}

void SettingsView::boardSpeedDecreaseFunction(){
	boardSpeed--;
	boardSpeed = boardSpeed < 1 ? 1 : boardSpeed;
	setBoardSpeed(boardSpeed);
}

void SettingsView::scoreLimitIncreaseFunction(){
	scoreLimit++;
	setScoreLimit(scoreLimit);
}

void SettingsView::scoreLimitDecreaseFunction(){
	scoreLimit--;
	scoreLimit = scoreLimit < 1 ? 1 : scoreLimit;
	setScoreLimit(scoreLimit);
}

void SettingsView::playButtonHandleFunction(){

}

void SettingsView::resetButtonHandleFuction(){
	ballSpeed = 2;
	boardSpeed = 2;
	scoreLimit = 10;
	setBallSpeed(ballSpeed);
	setBoardSpeed(boardSpeed);
	setScoreLimit(scoreLimit);
}

void SettingsView::saveButtonHandleFunction(){

}

void SettingsView::setBallSpeed(int num)
{
	Unicode::snprintf(ballSpeedValueBuffer, 10, "%d", num);
	ballSpeedValue.setWildcard(ballSpeedValueBuffer);
	ballSpeedValue.invalidate(); // Refresh the text area to display the new value
}

void SettingsView::setBoardSpeed(int num)
{
	Unicode::snprintf(boardSpeedValueBuffer, 10, "%d", num);
	boardSpeedValue.setWildcard(boardSpeedValueBuffer);
	boardSpeedValue.invalidate(); // Refresh the text area to display the new value
}

void SettingsView::setScoreLimit(int num)
{
	Unicode::snprintf(scoreLimitValueBuffer, 10, "%d", num);
	scoreLimitValue.setWildcard(scoreLimitValueBuffer);
	scoreLimitValue.invalidate(); // Refresh the text area to display the new value
}
