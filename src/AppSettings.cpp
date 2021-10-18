#include "AppSettings.h"
#include "EEPROM.h"

AppSettings::AppSettings()
{
    needInit = false;
}

void AppSettings::load()
{
    AppSettingsData data;
    EEPROM.get<AppSettingsData>(0, data);

    if (data.magicId == 0xFEEDC0DE)
    {
        needInit = data.needInit;
        // TODO Init from data.
    }
    else
    {
        needInit = true;
    }
}

void AppSettings::editCurrent()
{
    needInit = true;
    updateInFlash();
    ESP.restart();
}

void AppSettings::commit() 
{
    needInit = false;
    updateInFlash();
}

void AppSettings::updateInFlash()
{
    AppSettingsData data;
    data.magicId = 0xFEEDC0DE;
    data.needInit = needInit;

    // TODO Set data.

    EEPROM.put(0, data);
    EEPROM.commit();
}
