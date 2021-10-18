#include "AppStatus.h"

AppStatus::AppStatus()
{
    xMutex_appStatus = xSemaphoreCreateMutex();
    
    batVoltage = 0;
    extPower = false;
    powerOffScheduled = false;

    heartBeat = false;

    carBatVoltage = 0;
    engineCoolantTemp = 0;
    engineRpm = 0;
    engineLoad = 0;
    acceleration = 0;
    bluetoothConnected = false;
    elm327Connected = false;
    lastElm327Status = -1000;

    screenRotation = 3;
    screenRotationUpdated = false;
}

void AppStatus::lock()
{
    while (xSemaphoreTake(xMutex_appStatus, 1 / portTICK_PERIOD_MS) != pdTRUE)
    {
        delayMicroseconds(1);
    }
}

void AppStatus::unlock()
{
    xSemaphoreGive(xMutex_appStatus);
}

void AppStatus::mutualUpdate(const std::function<void(AppStatus *)> action)
{
    lock();
    action(this);
    unlock();
}
