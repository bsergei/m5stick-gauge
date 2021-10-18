#ifndef __APPUI_H__
#define __APPUI_H__

#define UI_STATUS 1
#define UI_STATUS_WARMUP 2

#include <M5StickCPlus.h>
#include "AppStatus.h"
#include "AppSettings.h"

class AppUI
{
public:
    AppUI(AppStatus *t_appStatus, AppSettings *t_appSettings);
    ~AppUI();
    void begin();
    void update();

    void setMode(int m);

private:
    AppStatus *appStatus;
    AppSettings *appSettings;
    TFT_eSprite disbuff;
    bool started;
    int page;
    int mode;

    int rotation;

    void handlePageUpdate();
    void updatePage(int page, bool pageEnter, bool pageExit);
    void handleAutoRotation();

    int getModePageCount();
   
    void printPageStatus(bool pageEnter, bool pageExit);
    
    void displaybuff();
    void clear();
};

#endif