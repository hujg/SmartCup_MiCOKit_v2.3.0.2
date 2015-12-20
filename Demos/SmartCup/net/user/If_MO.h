


/***

History:
2015-11-30: Ted: Create

*/

#ifndef _IF_MO_H
#define _IF_MO_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "Object_int.h"
#include "stdbool.h"
   

typedef struct SDevice_t {
    u8      power;
    i16     signalStrength;
    float   temperature;
    float   tfCapacity;
    float   tfFree;
    bool    lowPowerAlarm;
    bool    powerOnDisplay;
    bool    tfStatus;
    bool    tempSwitch;
} SDevice;
   

typedef struct SLight_t {
    u8      lightsType;
    u8      redConf;
    u8      greenConf;
    u8      blueConf;
    bool    enableNotifyLight;
    bool    ledSwitch;
} SLight;


typedef struct SMusic_t {
    u8      volume;
    u8      downLoadRate;
    u16     trackNumber;
    u16     delTrack;
    bool    getTrackList;
    char*   urlPath;
} SMusic;


typedef struct STrack_t {
    u16     trackIdx;
    char*   trackName;
} STrack;


typedef struct SHealth_t {
    u32     drinkStamp;
    u32     putDownStamp;
    u8      remindType;
    u8      remindTimes;
    u8      noDisturbingStartHour;
    u8      noDisturbingStartMinute;
    u8      noDisturbingEndHour;
    u8      noDisturbingEndMinute;
    bool    ifNoDisturbing;
} SHealth;


#define MAX_DEPTH_PICKUP    8

typedef struct SPickup_t {
    u16     selTrack;
    bool    enable;
} SPickup;


#define MAX_DEPTH_PUTDOWN   5

typedef struct SPutdown_t {
    u16     remindDelay;
    u16     selTrack;
    bool    enable;
} SPutdown;


#define MAX_DEPTH_IMMEDIATE     1

typedef struct SImmediate_t {
    u16     selTrack;
    bool    enable;
} SImmediate;


#define MAX_DEPTH_SCHEDULE  5

typedef struct SSchedule_t {
    u16     remindHour;
    u16     remindMinute;
    u16     selTrack;
    bool    enable;
} SSchedule;


void MOInit();
void SetPower(u8 power);
u8 GetPower();
bool IsPowerChanged();
void SetLowPowerAlarm(bool flag);
bool GetLowPowerAlarm();
bool IsLowPowerAlarmChanged();
void SetSignalStrengh(i16 value);
i16 GetSignalStrengh();
bool IsSignalStrenghChanged();
void SetTemperature(float value);
float GetTemperature();
bool IsTemperatureChanged();
void SetTempSwitch(bool flag);
bool GetTempSwitch();
bool IsTempSwitchChanged();
void SetPowerOnDisplay(bool flag);
bool GetPowerOnDisplay();
bool IsPowerOnDisplayChanged();
void SetTFStatus(bool flag);
bool GetTFStatus();
bool IsTFStatusChanged();
void SetTFCapacity(float value);
float GetTFCapacity();
bool IsTFCapacityChanged();
void SetTFFree(float value);
float GetTFFree();
bool IsTFFreeChanged();
void SetLightsType(u8 value);
u8 GetLightsType();
bool IsLightsTypeChanged();
void SetEnableNotifyLight(bool flag);
bool GetEnableNotifyLight();
bool IsEnableNotifyLightChanged();
void SetLedSwitch(bool flag);
bool GetLedSwitch();
bool IsLedSwitchChanged();
void SetRedConf(u8 value);
u8 GetRedConf();
bool IsRedConfChanged();
void SetGreenConf(u8 value);
u8 GetGreenConf();
bool IsGreenConfChanged();
void SetBlueConf(u8 value);
u8 GetBlueConf();
bool IsBlueConfChanged();
void SetVolume(u8 value);
u8 GetVolume();
bool IsVolumeChanged();
void SetDownLoadRate(u8 value);
u8 GetDownLoadRate();
bool IsDownLoadRateChanged();
void SetDrinkStamp(u32 value);
u32 GetDrinkStamp();
bool IsDrinkStampChanged();
void SetPutDownStamp(u32 value);
u32 GetPutDownStamp();
bool IsPutDownStampChanged();
void SetRemindType(u8 value);
u8 GetRemindType();
bool IsRemindTypeChanged();
void SetRemindTimes(u8 value);
u8 GetRemindTimes();
bool IsRemindTimesChanged();
void SetIfNoDisturbing(bool flag);
bool GetIfNoDisturbing();
bool IsIfNoDisturbingChanged();
void SetNoDisturbingStartHour(u8 value);
u8 GetNoDisturbingStartHour();
bool IsNoDisturbingStartHourChanged();
void SetNoDisturbingStartMinute(u8 value);
u8 GetNoDisturbingStartMinute();
bool IsNoDisturbingStartMinuteChanged();
void SetNoDisturbingEndHour(u8 value);
u8 GetNoDisturbingEndHour();
bool IsNoDisturbingEndHourChanged();
void SetNoDisturbingEndMinute(u8 value);
u8 GetNoDisturbingEndMinute();
bool IsNoDisturbingEndMinuteChanged();
void SetPickUp(u8 index, bool enable, u16 track);
void SetPutDown(u8 index, bool enable, u16 track, u16 delay);


   
#ifdef __cplusplus
}
#endif

#endif // _IF_MO_H

// end of file


