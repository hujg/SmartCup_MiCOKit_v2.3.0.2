/**
******************************************************************************
* @file    user_main.c 
* @author  Eshen Wang
* @version V1.0.0
* @date    14-May-2015
* @brief   user main functons in user_main thread.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MXCHIP Inc. SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2014 MXCHIP Inc.</center></h2>
******************************************************************************
*/ 

#include "mico.h"
#include "MicoFogCloud.h"
#include "json_c/json.h"
#include "If_MO.h"
#include "DeviceMonitor.h"
#include "TimeUtils.h"


/* User defined debug log functions
 * Add your own tag like: 'USER_UPSTREAM', the tag will be added at the beginning of a log
 * in MICO debug uart, when you call this function.
 */
#define user_log(M, ...) custom_log("USER_UPSTREAM", M, ##__VA_ARGS__)
#define user_log_trace() custom_log_trace("USER_UPSTREAM")

static void DoCycleOneSecond();
static void MOChangedNotification();
static void SendJsonInt(app_context_t *arg, char* str, int value);
static void SendJsonDouble(app_context_t *arg, char* str, double value);
static void SendJsonBool(app_context_t *arg, char* str, bool value);


  
/* Message upload thread
 */
void user_upstream_thread(void* arg)
{
  user_log_trace();
  app_context_t *app_context = (app_context_t *)arg;
  OSStatus err = kUnknownErr;
  //uint8_t ret = 0;
    
  require(app_context, exit);
  
  /* thread loop */
  while(1){

    if(app_context->appStatus.fogcloudStatus.isCloudConnected == false) {
        user_log("user_upstream_thread: cloud disconnected");
        mico_thread_sleep(2);
        continue;
    }

    MOChangedNotification(app_context);

    DoCycleOneSecond();
  }

exit:
  if(kNoErr != err){
    user_log("ERROR: user_uptream exit with err=%d", err);
  }
  mico_rtos_delete_thread(NULL);  // delete current thread
}

static void DoCycleOneSecond()
{
    static u32 count = 0;

    while(count <= UpTicks()) {
        count = UpTicks() + 1*UpTicksPerSecond();

        PowerNotification();
        LowPowerAlarmNotification();
        SignalStrengthNotification();
        TFCardNotification();
    }
}

static void MOChangedNotification(app_context_t *app_context)
{
    if(IsPowerChanged()) {
        SendJsonInt(app_context, "DEVICE-1/Power", GetPower());
    }
    else if(IsLowPowerAlarmChanged()) {
        SendJsonBool(app_context, "DEVICE-1/LowPowerAlarm", GetLowPowerAlarm());
    }
    else if(IsSignalStrenghChanged()) {
        SendJsonInt(app_context, "DEVICE-1/SignalStrength", GetSignalStrengh());
    }
    else if(IsTemperatureChanged()) {
        SendJsonDouble(app_context, "DEVICE-1/Temperature", GetTemperature());
    }
    else if(IsTFStatusChanged()) {
        SendJsonBool(app_context, "DEVICE-1/TFStatus", GetTFStatus());
    }
    else if(IsTFCapacityChanged()) {
        SendJsonDouble(app_context, "DEVICE-1/TFCapacity", GetTFCapacity());
    }
    else if(IsTFFreeChanged()) {
        SendJsonDouble(app_context, "DEVICE-1/TFFree", GetTFFree());
    }
    else if(IsDownLoadRateChanged()) {
        SendJsonInt(app_context, "MUSIC-1/DownLoadRate", GetDownLoadRate());
    }
    else if(IsDrinkStampChanged()) {
        SendJsonInt(app_context, "HEALTH-1/DrinkStamp", GetDrinkStamp());
    }
    else if(IsPutDownStampChanged()) {
        SendJsonInt(app_context, "HEALTH-1/PutDownStamp", GetPutDownStamp());
    }
}

static void SendJsonInt(app_context_t *arg, char* str, int value)
{
    app_context_t *app_context = (app_context_t *)arg;
    json_object *send_json_object = NULL;
    const char *upload_data = NULL;
  
    send_json_object = json_object_new_object();
    if(NULL == send_json_object){
        user_log("create json object error!");
    }
    else {
        json_object_object_add(send_json_object, str, json_object_new_int(value)); 
        upload_data = json_object_to_json_string(send_json_object);
        if(NULL == upload_data) {
            user_log("create upload data string error!");
        }
        else {
            // upload data string to fogcloud, the seconde param(NULL) means send to defalut topic: '<device_id>/out'
            MiCOFogCloudMsgSend(app_context, NULL, (unsigned char*)upload_data, strlen(upload_data));
            user_log("upload data success! \t topic=%s/out \t %s", 
                    app_context->appConfig->fogcloudConfig.deviceId,
                    upload_data);
        }
    }

    // free json object memory
    json_object_put(send_json_object);
    send_json_object = NULL;
}

static void SendJsonDouble(app_context_t *arg, char* str, double value)
{
    app_context_t *app_context = (app_context_t *)arg;
    json_object *send_json_object = NULL;
    const char *upload_data = NULL;
  
    send_json_object = json_object_new_object();
    if(NULL == send_json_object){
        user_log("create json object error!");
    }
    else {
        json_object_object_add(send_json_object, str, json_object_new_double(value)); 
        upload_data = json_object_to_json_string(send_json_object);
        if(NULL == upload_data) {
            user_log("create upload data string error!");
        }
        else {
            // upload data string to fogcloud, the seconde param(NULL) means send to defalut topic: '<device_id>/out'
            MiCOFogCloudMsgSend(app_context, NULL, (unsigned char*)upload_data, strlen(upload_data));
            user_log("upload data success! \t topic=%s/out \t %s", 
                    app_context->appConfig->fogcloudConfig.deviceId,
                    upload_data);
        }
    }

    // free json object memory
    json_object_put(send_json_object);
    send_json_object = NULL;
}

static void SendJsonBool(app_context_t *arg, char* str, bool value)
{
    app_context_t *app_context = (app_context_t *)arg;
    json_object *send_json_object = NULL;
    const char *upload_data = NULL;
  
    send_json_object = json_object_new_object();
    if(NULL == send_json_object){
        user_log("create json object error!");
    }
    else {
        json_object_object_add(send_json_object, str, json_object_new_boolean(value)); 
        upload_data = json_object_to_json_string(send_json_object);
        if(NULL == upload_data) {
            user_log("create upload data string error!");
        }
        else {
            // upload data string to fogcloud, the seconde param(NULL) means send to defalut topic: '<device_id>/out'
            MiCOFogCloudMsgSend(app_context, NULL, (unsigned char*)upload_data, strlen(upload_data));
            user_log("upload data success! \t topic=%s/out \t %s", 
                    app_context->appConfig->fogcloudConfig.deviceId,
                    upload_data);
        }
    }

    // free json object memory
    json_object_put(send_json_object);
    send_json_object = NULL;
}



