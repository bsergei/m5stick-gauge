#include <Arduino.h>
#include <M5StickCPlus.h>

#include "MutualM5.h"

MutualM5::MutualM5()
{
  xMutex_Wire1 = xSemaphoreCreateMutex();
}

void MutualM5::lockWire1()
{
  while (xSemaphoreTake(xMutex_Wire1, 1 / portTICK_PERIOD_MS) != pdTRUE)
  {
    delayMicroseconds(1);
  }
}

void MutualM5::unlockWire1()
{
  xSemaphoreGive(xMutex_Wire1);
}

void MutualM5::ScreenBreath(uint8_t brightness)
{
  lockWire1();
  M5.Axp.ScreenBreath(brightness);
  unlockWire1();
}

float MutualM5::GetBatVoltage()
{
  lockWire1();
  float vbat = M5.Axp.GetBatVoltage();
  unlockWire1();
  return vbat;
}

uint8_t MutualM5::GetBtnPress()
{
  lockWire1();
  uint8_t res = M5.Axp.GetBtnPress();
  unlockWire1();
  return res;
}

void MutualM5::SetDateAndTime(RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
  lockWire1();
  M5.Rtc.SetData(date);
  M5.Rtc.SetTime(time);
  unlockWire1();
}

void MutualM5::GetDateAndTime(RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
  lockWire1();
  M5.Rtc.GetData(date);
  M5.Rtc.GetTime(time);
  unlockWire1();
}

float MutualM5::GetAccX()
{
  lockWire1();

  float accX = 0.0F;
  float accY = 0.0F;
  float accZ = 0.0F;

  M5.IMU.getAccelData(&accX, &accY, &accZ);

  unlockWire1();

  return accX;
}

bool MutualM5::IsExternalPowerConnected()
{
  lockWire1();

  bool isConnected = 
    M5.Axp.GetVBusVoltage() > 1.5
    || M5.Axp.GetVinVoltage() > 1.5;

  unlockWire1();

  return isConnected;
}

void MutualM5::PowerOff()
{
  lockWire1();

  M5.Axp.PowerOff();

  unlockWire1();
}

MutualM5 M5_Safe;
