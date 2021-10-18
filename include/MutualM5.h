#ifndef __MUTUALM5_H__
#define __MUTUALM5_H__

#include <Arduino.h>
#include <M5StickCPlus.h>

class MutualM5
{
public:
    MutualM5();

    void ScreenBreath(uint8_t brightness);
    float GetBatVoltage();
    uint8_t GetBtnPress(void);
    void SetDateAndTime(RTC_DateTypeDef *date, RTC_TimeTypeDef *time);
    void GetDateAndTime(RTC_DateTypeDef *date, RTC_TimeTypeDef *time);
    float GetAccX();
    bool IsExternalPowerConnected();
    void PowerOff();

private:
    SemaphoreHandle_t xMutex_Wire1;

    void lockWire1();
    void unlockWire1();
};

extern MutualM5 M5_Safe;

#endif
