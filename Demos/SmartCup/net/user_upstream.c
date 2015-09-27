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
#include <ObjDevice.h>
#include <ObjMusic.h>
#include <ObjLights.h>

/* User defined debug log functions
 * Add your own tag like: 'USER_UPSTREAM', the tag will be added at the beginning of a log
 * in MICO debug uart, when you call this function.
 */
#define user_log(M, ...) custom_log("USER_UPSTREAM", M, ##__VA_ARGS__)
#define user_log_trace() custom_log_trace("USER_UPSTREAM")


//
extern bool subscribe;
extern char* track;
extern char* url_path;
  
/* Message upload thread
 * Get DHT11 temperature/humidity data && upload to cloud
 */
void user_upstream_thread(void* arg)
{
  user_log_trace();
  
  OSStatus err = kUnknownErr;
  app_context_t *app_context = (app_context_t *)arg;
  json_object *send_json_object = NULL;
  const char *upload_data = NULL;
  u16 value = 10;
  char str[16] = NULL;
    
  require(app_context, exit);
  
  user_log("user_upstream_thread: Start");
  
  /* thread loop */
  while(1){
    if(subscribe) {
        if(!MiCOFogCloudIsActivated(app_context)) {
            user_log("appStatus.fogcloudStatus.isCloudConnected = false");
            mico_thread_sleep(1);
            continue;
        }
        
        // create json object to format upload data
        send_json_object = json_object_new_object();
        if(NULL == send_json_object) {
            user_log("create json object error!");
            err = kNoMemoryErr;
        }
        else {
            user_log("user_upstream_thread: Start upload Object Value");
            // add device parameter data into json object
            GetEnergyValue(&value);
            json_object_object_add(send_json_object, "energy", json_object_new_int(value));
            GetIntervalValue(&value);
            json_object_object_add(send_json_object, "interval", json_object_new_int(value));
            GetLightsValue(&value);
            json_object_object_add(send_json_object, "lights", json_object_new_int(value));
            GetRemindValue(&value);
            json_object_object_add(send_json_object, "remind", json_object_new_int(value));
            GetVolumeValue(&value);
            json_object_object_add(send_json_object, "volume", json_object_new_int(value));
            json_object_object_add(send_json_object, "subscribe", json_object_new_boolean(subscribe));
            memset(str, 0, sizeof(str));
            GetTrackString(str);
            json_object_object_add(send_json_object, "track", json_object_new_string_len(str, strlen(str)));
            memset(str, 0, sizeof(str));
            GetUrlPathString(str);
            json_object_object_add(send_json_object, "url_path", json_object_new_string_len(str, strlen(str)));
            
            upload_data = json_object_to_json_string(send_json_object);
            if(NULL == upload_data) {
                user_log("create upload data string error!");
                err = kNoMemoryErr;
            }
            else {
                // upload data string to fogcloud, the seconde param(NULL) means send to defalut topic: '<device_id>/out'
                MiCOFogCloudMsgSend(app_context, NULL, (unsigned char*)upload_data, strlen(upload_data));
                user_log("ready sended, <topic:%s> <data:%s>", app_context->appConfig->fogcloudConfig.deviceId, (unsigned char*)upload_data);
                err = kNoErr;
            }
            
            // free json object memory
            json_object_put(send_json_object);
            send_json_object = NULL;
            user_log("free json_object_put");
        }
        
        mico_thread_sleep(10);
    }
  }

exit:
  if(kNoErr != err){
    user_log("ERROR: user_uptream exit with err=%d", err);
  }
  mico_rtos_delete_thread(NULL);  // delete current thread
}
