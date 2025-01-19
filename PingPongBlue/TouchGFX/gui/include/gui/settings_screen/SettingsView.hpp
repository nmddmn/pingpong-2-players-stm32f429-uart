#ifndef SETTINGSVIEW_HPP
#define SETTINGSVIEW_HPP

#include <gui_generated/settings_screen/SettingsViewBase.hpp>
#include <gui/settings_screen/SettingsPresenter.hpp>

class SettingsView : public SettingsViewBase
{
public:
    SettingsView();
    virtual ~SettingsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void ballSpeedIncreaseFunction();
    void ballSpeedDecreaseFunction();
    void boardSpeedIncreaseFunction();
    void boardSpeedDecreaseFunction();
    void scoreLimitIncreaseFunction();
    void scoreLimitDecreaseFunction();
    void playButtonHandleFunction();
    void resetButtonHandleFuction();
    void saveButtonHandleFunction();

    void setBallSpeed(int num);
    void setBoardSpeed(int num);
    void setScoreLimit(int num);
protected:
};

#endif // SETTINGSVIEW_HPP
