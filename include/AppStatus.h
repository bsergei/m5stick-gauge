#ifndef __APPSTATUS_H__
#define __APPSTATUS_H__

#include <M5StickCPlus.h>

class AppStatus
{
public:
    AppStatus();

    float batVoltage;
    bool extPower;
    bool powerOffScheduled;

    bool heartBeat;

    float carBatVoltage;
    float engineCoolantTemp;
    float engineRpm;
    float engineLoad;
    float acceleration;
    
    bool bluetoothConnected;
    bool elm327Connected;
    int lastElm327Status;

    int screenRotation;
    bool screenRotationUpdated;

    void mutualUpdate(const std::function<void(AppStatus *)> action);

    template <typename T>
    T mutualRead(const std::function<T(AppStatus *)> func)
    {
        lock();
        T value = func(this);
        unlock();
        return value;
    }

    void lock();
    void unlock();

private:
    SemaphoreHandle_t xMutex_appStatus;
};

#endif