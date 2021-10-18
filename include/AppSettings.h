#ifndef __APPSETTINGS_H__
#define __APPSETTINGS_H__

struct AppSettingsData
{
    unsigned long magicId;

    bool needInit;
};

class AppSettings
{
public:
    AppSettings();

    void load();
    void editCurrent();
    void commit();

    bool needInit;
    
private:
    void updateInFlash();
};

#endif