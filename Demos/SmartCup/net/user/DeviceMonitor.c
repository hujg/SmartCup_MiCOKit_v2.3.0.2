


/***

History:
2015-12-20: Ted: Create

*/


#include "OMFactory.h"
#include "DeviceMonitor.h"
#include "mico.h"
#include "If_MO.h"


#define user_log(M, ...) custom_log("DeviceMonitor", M, ##__VA_ARGS__)
#define user_log_trace() custom_log_trace("DeviceMonitor")


void PowerNotification()
{
    SetPower(80);
}


#define LOW_POWER_LIMIT     15

void LowPowerAlarmNotification()
{
    SetLowPowerAlarm( GetPower() < LOW_POWER_LIMIT ? true : false );
}

void SignalStrengthNotification()
{
    OSStatus err = kNoErr;
    LinkStatusTypeDef wifi_link_status;
    
    err = micoWlanGetLinkStatus(&wifi_link_status);
    if(kNoErr != err){
      user_log("SignalStrengthNotification: err code(%d)", err);
    }

    if(GetSignalStrengh() != wifi_link_status.wifi_strength) {
        SetSignalStrengh(wifi_link_status.wifi_strength);
    }
}

void TFCardNotification()
{
    SetTFStatus(true);
    SetTFCapacity(4*1024);
    SetTFFree(3*1024);
}


// end of file


