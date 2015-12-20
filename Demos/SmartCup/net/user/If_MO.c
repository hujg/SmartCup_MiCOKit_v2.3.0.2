


/***

History:
2015-12-13: Ted: Create

*/


#include "If_MO.h"
#include "mico.h"
#include "OMFactory.h"


SDevice 	gDevice;
SLight  	gLight;
SMusic		gMusic;
STrack  	gTrack;
SHealth 	gHealth;
SPickup 	gPickup[MAX_DEPTH_PICKUP];
SPutdown	gPutdown[MAX_DEPTH_PUTDOWN];
SImmediate	gImmediate[MAX_DEPTH_IMMEDIATE];
SSchedule	gSchedule[MAX_DEPTH_SCHEDULE];

static bool isPowerChanged = false;
static bool isLowPowerAlarmChanged = false;
static bool isSignalStrenghChanged = false;
static bool isTemperatureChanged = false;
static bool isTempSwitchChanged = false;
static bool isPowerOnDisplayChanged = false;
static bool isTFStatusChanged = false;
static bool isTFCapacityChanged = false;
static bool isTFFreeChanged = false;
static bool isLightsTypeChanged = false;
static bool isEnableNotifyLightChanged = false;
static bool isLedSwitchChanged = false;
static bool isRedConfChanged = false;
static bool isGreenConfChanged = false;
static bool isBlueConfChanged = false;
static bool isVolumeChanged = false;
static bool isDownLoadRateChanged = false;
static bool isDrinkStampChanged = false;
static bool isPutDownStampChanged = false;
static bool isRemindTypeChanged = false;
static bool isRemindTimesChanged = false;
static bool isIfNoDisturbingChanged = false;
static bool isNoDisturbingStartHourChanged = false;
static bool isNoDisturbingStartMinuteChanged = false;
static bool isNoDisturbingEndHourChanged = false;
static bool isNoDisturbingEndMinuteChanged = false;
static bool isPickupChanged = false;
static bool isPutdownChanged = false;
static bool isImmediateChanged = false;
static bool isScheduleChanged = false;



static mico_mutex_t omMutex = NULL;

#define user_log(M, ...) custom_log("If_MO", M, ##__VA_ARGS__)
#define user_log_trace() custom_log_trace("If_MO")



void MOInit()
{
    OSStatus err;

    err = mico_rtos_init_mutex(&omMutex);
    if(err) {
        user_log("[ERR]MOInit: create mutex failed");
    }

    // init OM, get value from flash
    OMFactoryInit();
}

void SetPower(u8 power)
{
    mico_rtos_lock_mutex(&omMutex);

    if(power != gDevice.power) {
        gDevice.power = power;
        isPowerChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetPower()
{
    return gDevice.power;
}

bool IsPowerChanged()
{
    if(isPowerChanged) {
        isPowerChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetLowPowerAlarm(bool flag)
{
    mico_rtos_lock_mutex(&omMutex);

    if(flag != gDevice.lowPowerAlarm) {
        gDevice.lowPowerAlarm = flag;
        isLowPowerAlarmChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

bool GetLowPowerAlarm()
{
    return gDevice.lowPowerAlarm;
}

bool IsLowPowerAlarmChanged()
{
    if(isLowPowerAlarmChanged) {
        isLowPowerAlarmChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetSignalStrengh(i16 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gDevice.signalStrength) {
        gDevice.signalStrength = value;
        isSignalStrenghChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

i16 GetSignalStrengh()
{
    return gDevice.signalStrength;
}

bool IsSignalStrenghChanged()
{
    if(isSignalStrenghChanged) {
        isSignalStrenghChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetTemperature(float value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gDevice.temperature) {
        gDevice.temperature = value;
        isTemperatureChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

float GetTemperature()
{
    return gDevice.temperature;
}

bool IsTemperatureChanged()
{
    if(isTemperatureChanged) {
        isTemperatureChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetTempSwitch(bool flag)
{
    mico_rtos_lock_mutex(&omMutex);

    if(flag != gDevice.tempSwitch) {
        gDevice.tempSwitch = flag;
        isTempSwitchChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

bool GetTempSwitch()
{
    return gDevice.tempSwitch;
}

bool IsTempSwitchChanged()
{
    if(isTempSwitchChanged) {
        isTempSwitchChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetPowerOnDisplay(bool flag)
{
    mico_rtos_lock_mutex(&omMutex);

    if(flag != gDevice.powerOnDisplay) {
        gDevice.powerOnDisplay = flag;
        isPowerOnDisplayChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

bool GetPowerOnDisplay()
{
    return gDevice.powerOnDisplay;
}

bool IsPowerOnDisplayChanged()
{
    if(isPowerOnDisplayChanged) {
        isPowerOnDisplayChanged = false;
        return true;
    }
    else {
        return false;
    }
}


void SetTFStatus(bool flag)
{
    mico_rtos_lock_mutex(&omMutex);

    if(flag != gDevice.tfStatus) {
        gDevice.tfStatus = flag;
        isTFStatusChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

bool GetTFStatus()
{
    return gDevice.tfStatus;
}

bool IsTFStatusChanged()
{
    if(isTFStatusChanged) {
        isTFStatusChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetTFCapacity(float value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gDevice.tfCapacity) {
        gDevice.tfCapacity = value;
        isTFCapacityChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

float GetTFCapacity()
{
    return gDevice.tfCapacity;
}

bool IsTFCapacityChanged()
{
    if(isTFCapacityChanged) {
        isTFCapacityChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetTFFree(float value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gDevice.tfFree) {
        gDevice.tfFree = value;
        isTFFreeChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

float GetTFFree()
{
    return gDevice.tfFree;
}

bool IsTFFreeChanged()
{
    if(isTFFreeChanged) {
        isTFFreeChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetLightsType(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gLight.lightsType) {
        gLight.lightsType = value;
        isLightsTypeChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetLightsType()
{
    return gLight.lightsType;
}

bool IsLightsTypeChanged()
{   
    if(isLightsTypeChanged) {
        isLightsTypeChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetEnableNotifyLight(bool flag)
{
    mico_rtos_lock_mutex(&omMutex);

    if(flag != gLight.enableNotifyLight) {
        gLight.enableNotifyLight = flag;
        isEnableNotifyLightChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

bool GetEnableNotifyLight()
{
    return gLight.enableNotifyLight;
}

bool IsEnableNotifyLightChanged()
{   
    if(isEnableNotifyLightChanged) {
        isEnableNotifyLightChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetLedSwitch(bool flag)
{
    mico_rtos_lock_mutex(&omMutex);

    if(flag != gLight.ledSwitch) {
        gLight.ledSwitch = flag;
        isLedSwitchChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

bool GetLedSwitch()
{
    return gLight.ledSwitch;
}

bool IsLedSwitchChanged()
{   
    if(isLedSwitchChanged) {
        isLedSwitchChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetRedConf(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gLight.redConf) {
        gLight.redConf = value;
        isRedConfChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetRedConf()
{
    return gLight.redConf;
}

bool IsRedConfChanged()
{
    if(isRedConfChanged) {
        isRedConfChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetGreenConf(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gLight.greenConf) {
        gLight.greenConf= value;
        isGreenConfChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetGreenConf()
{
    return gLight.greenConf;
}

bool IsGreenConfChanged()
{
    if(isGreenConfChanged) {
        isGreenConfChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetBlueConf(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gLight.blueConf) {
        gLight.blueConf= value;
        isBlueConfChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetBlueConf()
{
    return gLight.blueConf;
}

bool IsBlueConfChanged()
{
    if(isBlueConfChanged) {
        isBlueConfChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetVolume(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gMusic.volume) {
        gMusic.volume= value;
        isVolumeChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetVolume()
{
    return gMusic.volume;
}

bool IsVolumeChanged()
{
    if(isVolumeChanged) {
        isVolumeChanged = false;
        return true;
    }
    else {
        return false;
    }
}


void SetDownLoadRate(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gMusic.downLoadRate) {
        gMusic.downLoadRate = value;
        isDownLoadRateChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetDownLoadRate()
{
    return gMusic.downLoadRate;
}

bool IsDownLoadRateChanged()
{   
    if(isDownLoadRateChanged) {
        isDownLoadRateChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetDrinkStamp(u32 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gHealth.drinkStamp) {
        gHealth.drinkStamp = value;
        isDrinkStampChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u32 GetDrinkStamp()
{
    return gHealth.drinkStamp;
}

bool IsDrinkStampChanged()
{
    if(isDrinkStampChanged) {
        isDrinkStampChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetPutDownStamp(u32 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gHealth.putDownStamp) {
        gHealth.putDownStamp = value;
        isPutDownStampChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u32 GetPutDownStamp()
{
    return gHealth.putDownStamp;
}

bool IsPutDownStampChanged()
{
    if(isPutDownStampChanged) {
        isPutDownStampChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetRemindType(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gHealth.remindType) {
        gHealth.remindType = value;
        isRemindTypeChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetRemindType()
{
    return gHealth.remindType;
}

bool IsRemindTypeChanged()
{
    if(isRemindTypeChanged) {
        isRemindTypeChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetRemindTimes(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gHealth.remindTimes) {
        gHealth.remindTimes = value;
        isRemindTimesChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetRemindTimes()
{
    return gHealth.remindTimes;
}

bool IsRemindTimesChanged()
{
    if(isRemindTimesChanged) {
        isRemindTimesChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetIfNoDisturbing(bool flag)
{
    mico_rtos_lock_mutex(&omMutex);

    if(flag != gHealth.ifNoDisturbing) {
        gHealth.ifNoDisturbing = flag;
        isIfNoDisturbingChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

bool GetIfNoDisturbing()
{
    return gHealth.ifNoDisturbing;
}

bool IsIfNoDisturbingChanged()
{
    if(isIfNoDisturbingChanged) {
        isIfNoDisturbingChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetNoDisturbingStartHour(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gHealth.noDisturbingStartHour) {
        gHealth.noDisturbingStartHour = value;
        isNoDisturbingStartHourChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetNoDisturbingStartHour()
{
    return gHealth.noDisturbingStartHour;
}

bool IsNoDisturbingStartHourChanged()
{
    if(isNoDisturbingStartHourChanged) {
        isNoDisturbingStartHourChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetNoDisturbingStartMinute(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gHealth.noDisturbingStartMinute) {
        gHealth.noDisturbingStartMinute = value;
        isNoDisturbingStartMinuteChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetNoDisturbingStartMinute()
{
    return gHealth.noDisturbingStartMinute;
}

bool IsNoDisturbingStartMinuteChanged()
{
    if(isNoDisturbingStartMinuteChanged) {
        isNoDisturbingStartMinuteChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetNoDisturbingEndHour(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gHealth.noDisturbingEndHour) {
        gHealth.noDisturbingEndHour = value;
        isNoDisturbingEndHourChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetNoDisturbingEndHour()
{
    return gHealth.noDisturbingEndHour;
}

bool IsNoDisturbingEndHourChanged()
{
    if(isNoDisturbingEndHourChanged) {
        isNoDisturbingEndHourChanged = false;
        return true;
    }
    else {
        return false;
    }
}

void SetNoDisturbingEndMinute(u8 value)
{
    mico_rtos_lock_mutex(&omMutex);

    if(value != gHealth.noDisturbingEndMinute) {
        gHealth.noDisturbingEndMinute = value;
        isNoDisturbingEndMinuteChanged = true;
    }

    mico_rtos_unlock_mutex(&omMutex);
}

u8 GetNoDisturbingEndMinute()
{
    return gHealth.noDisturbingEndMinute;
}

bool IsNoDisturbingEndMinuteChanged()
{
    if(isNoDisturbingEndMinuteChanged) {
        isNoDisturbingEndMinuteChanged = false;
        return true;
    }
    else {
        return false;
    }
}



void SetNoDisturbingTime(u8 sh, u8 eh, u8 sm, u8 em)
{
    mico_rtos_lock_mutex(&omMutex);

    

    mico_rtos_unlock_mutex(&omMutex);
}

void SetPickUp(u8 index, bool enable, u16 track)
{
    mico_rtos_lock_mutex(&omMutex);
    
    if(index >= MAX_DEPTH_PICKUP) {
        user_log("[ERR]SetPickUp: index(%d) exceed the depth(%d)", index, MAX_DEPTH_PICKUP);
        return ;
    }

    gPickup[index].enable = enable;
    gPickup[index].selTrack = track;

    isPickupChanged = true;

    mico_rtos_unlock_mutex(&omMutex);
}

bool IsPickupChanged()
{
    bool flage = isPickupChanged;
    isPickupChanged = false;
    return false;
}

void SetPutDown(u8 index, bool enable, u16 track, u16 delay)
{
    mico_rtos_lock_mutex(&omMutex);
    
    if(index >= MAX_DEPTH_PUTDOWN) {
        user_log("[ERR]SetPutDown: index(%d) exceed the depth(%d)", index, MAX_DEPTH_PUTDOWN);
        return ;
    }

    gPutdown[index].enable = enable;
    gPutdown[index].selTrack = track;
    gPutdown[index].remindDelay = delay;

    isPutdownChanged = true;

    mico_rtos_unlock_mutex(&omMutex);
}


// end of file


