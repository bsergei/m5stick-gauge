#include <ELMduino.h>
#include "AppUI.h"
#include "AppStatus.h"
#include "AppSettings.h"
#include "MutualM5.h"
#include "Icon.h"

AppUI::AppUI(AppStatus *t_appStatus, AppSettings *t_appSettings) : appStatus(t_appStatus),
                                                                   appSettings(t_appSettings),
                                                                   disbuff(TFT_eSprite(&M5.Lcd)),
                                                                   started(false),
                                                                   page(0),
                                                                   mode(0)
{
}

AppUI::~AppUI()
{
}

void AppUI::setMode(int m)
{
    mode = m;
}

void AppUI::begin()
{
    started = true;
    rotation = 3;
    M5.Lcd.setRotation(rotation);
    disbuff.createSprite(240, 135);

    // if (appSettings->needInit)
    // {
    //     setMode(1);
    // }
    // else
    // {
    //     M5.update();
    //     if (M5.BtnA.isPressed())
    //     {
    //         setMode(2);
    //     }
    // }

    page = 0;
    updatePage(page, true, false);
}

void AppUI::handleAutoRotation()
{
    if (appStatus->screenRotationUpdated)
    {
        appStatus->screenRotationUpdated = false;
        M5.Lcd.setRotation(appStatus->screenRotation);
    }
}

void AppUI::update()
{
    appStatus->lock();

    handleAutoRotation();

    if (!started)
    {
        begin();
    }

    handlePageUpdate();
    updatePage(page, false, false);

    appStatus->unlock();
}

int AppUI::getModePageCount()
{
    switch (mode)
    {
    // Normal
    case 0:
        return 1;
    // Init
    case 1:
        return 0;
    // Normal/Config
    case 2:
        return 0;
    default:
        return 0;
    }
}

void AppUI::updatePage(int page, bool pageEnter, bool pageExit)
{
    switch (mode)
    {
        case 0:
            switch (page)
            {
                case 0:
                    printPageStatus(pageEnter, pageExit);
                    break;
            }
            break;
    }
}

void AppUI::handlePageUpdate()
{
    M5.update();
    if (M5.BtnA.wasPressed())
    {
        int lastPage = page;
        page++;
        if (page >= getModePageCount())
        {
            page = 0;
        }

        if (lastPage != page)
        {
            updatePage(page, false, true);
        }

        updatePage(page, true, false);
    }
}

void AppUI::printPageStatus(bool pageEnter, bool pageExit)
{
    disbuff.fillRect(0, 0, 240, 135, disbuff.color565(0, 0, 0));
    
    // --> Status bar

    disbuff.setTextColor(TFT_WHITE);
    disbuff.setTextSize(1);
    disbuff.fillRect(0, 120, 240, 15, disbuff.color565(20, 20, 20));

    if (appStatus->heartBeat)
    {
        disbuff.fillRect(230, 125, 5, 5, TFT_GREEN);
    }

    disbuff.setCursor(190, 125);
    disbuff.printf("B:%.2f", appStatus->batVoltage);

    disbuff.setCursor(165, 125);
    disbuff.printf(appStatus->bluetoothConnected ? "+BT" : "-BT");

    disbuff.setCursor(135, 125);
    disbuff.printf(appStatus->elm327Connected ? "+327" : "-327");

    // <-- Status bar

    bool isConnected = appStatus->bluetoothConnected && appStatus->elm327Connected;

    if (isConnected)
    {
        // Logo
        disbuff.pushImage(5, 10,
                          bmw_logo.width,
                          bmw_logo.height,
                          (uint16_t *)bmw_logo.pixel_data);

        // Engine temp
        disbuff.pushImage(85, 10,
                          engine_temp_icon.width,
                          engine_temp_icon.height,
                          (uint16_t *)engine_temp_icon.pixel_data);

        disbuff.setTextSize(4);
        disbuff.setTextColor(TFT_GREENYELLOW);
        disbuff.setCursor(135, 15);
        disbuff.printf("%-4d", (int)appStatus->engineCoolantTemp);

        // Engine RPM
        disbuff.pushImage(85, 50,
                          rpm_icon.width,
                          rpm_icon.height,
                          (uint16_t *)rpm_icon.pixel_data);

        disbuff.setTextSize(4);
        disbuff.setTextColor(TFT_GREENYELLOW);
        disbuff.setCursor(135, 55);
        disbuff.printf("%-4d", (int)appStatus->engineRpm);

        // --> Progress bar

        float width1 = min(240.0, (240.0 * appStatus->engineLoad / 100.0));
        disbuff.fillRect((240.0 - width1) / 2.0, 100, width1, 10, TFT_CYAN);

        float width2 = min(240.0, (240.0 * abs(appStatus->acceleration) / 100.0));
        uint32_t accColor = appStatus->acceleration > 0 ? TFT_RED : TFT_GREEN;
        disbuff.fillRect((240.0 - width2) / 2.0, 110, width2, 10, accColor);

        // <-- Progress bar

        // --> Status bar

        disbuff.setCursor(5, 125);
        disbuff.setTextSize(1);
        disbuff.setTextColor(TFT_WHITE);
        disbuff.printf("BAT:%.2f", appStatus->carBatVoltage);

        // <-- Status bar
    }
    else
    {
        disbuff.pushImage(58, 20,
                          not_connected.width,
                          not_connected.height,
                          (uint16_t *)not_connected.pixel_data);
    }

    disbuff.setCursor(80, 125);
    disbuff.setTextSize(1);
    disbuff.setTextColor(TFT_WHITE);

    if (!appStatus->bluetoothConnected || appStatus->lastElm327Status == -1000)
    {
        disbuff.printf("WAITING");
    }
    else
    {
        switch (appStatus->lastElm327Status)
        {
            case ELM_SUCCESS:
                disbuff.printf("OK");
                break;

            case ELM_NO_RESPONSE:
                disbuff.printf("NO_RESP");
                break;

            case ELM_STOPPED:
                disbuff.printf("STOPPED");
                break;

            case ELM_TIMEOUT:
                disbuff.printf("TIMEOUT");
                break;

            case ELM_NO_DATA:
                disbuff.printf("NO_DATA");
                break;

            case ELM_UNABLE_TO_CONNECT:
                disbuff.printf("NO_CONN");
                break;

            default:
                disbuff.printf("ERROR %d", appStatus->lastElm327Status);
                break;
        }
    }

    displaybuff();
}

void AppUI::clear()
{
    disbuff.fillRect(0, 0, 240, 135, disbuff.color565(0, 0, 0));
    displaybuff();
}

void AppUI::displaybuff()
{
    disbuff.pushSprite(0, 0);
}