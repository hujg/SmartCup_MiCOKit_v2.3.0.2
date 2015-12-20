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


/* User defined debug log functions
 * Add your own tag like: 'USER_DOWNSTREAM', the tag will be added at the beginning of a log
 * in MICO debug uart, when you call this function.
 */
#define user_log(M, ...) custom_log("USER_DOWNSTREAM", M, ##__VA_ARGS__)
#define user_log_trace() custom_log_trace("USER_DOWNSTREAM")


extern SDevice gDevice;

/* Handle user message from cloud
 * Receive msg from cloud && do hardware operation, like rgbled
 */
void user_downstream_thread(void* arg)
{
  user_log_trace();
  OSStatus err = kUnknownErr;
  app_context_t *app_context = (app_context_t *)arg;
  fogcloud_msg_t *recv_msg = NULL;
  json_object *recv_json_object = NULL;
      
  require(app_context, exit);
  
  /* thread loop to handle cloud message */
  while(1){
    mico_thread_sleep(1);
        
    // check fogcloud connect status
    if(!app_context->appStatus.fogcloudStatus.isCloudConnected){
      user_log("[ERR]user_downstream_thread: cloud disconnected");
      mico_thread_sleep(2);
      continue;
    }
    
    /* get a msg pointer, points to the memory of a msg: 
     * msg data format: recv_msg->data = <topic><data>
     */
    err = MiCOFogCloudMsgRecv(app_context, &recv_msg, 100);
    if(kNoErr == err){
      // debug log in MICO dubug uart
      user_log("Cloud => Module: topic[%d]=[%.*s]\tdata[%d]=[%.*s]", 
               recv_msg->topic_len, recv_msg->topic_len, recv_msg->data, 
               recv_msg->data_len, recv_msg->data_len, recv_msg->data + recv_msg->topic_len);
      
      // parse json data from the msg, get led control value
      recv_json_object = json_tokener_parse((const char*)(recv_msg->data + recv_msg->topic_len));
      if (NULL != recv_json_object){
        json_object_object_foreach(recv_json_object, key, val) {
          // parse
          if(strcmp(key, "DEVICE-1/TempSwitch") == 0) {
            SetTempSwitch(json_object_get_boolean(val));
          }
          else if(strcmp(key, "DEVICE-1/PowerOnDisplay") == 0) {
            SetPowerOnDisplay(json_object_get_boolean(val));
          }
          else if(strcmp(key, "LIGHT-1/LightType") == 0) {
            SetLightsType(json_object_get_int(val));
          }
          else if(strcmp(key, "LIGHT-1/EnableNotifyLight") == 0) {
            SetEnableNotifyLight(json_object_get_boolean(val));
          }
          else if(strcmp(key, "LIGHT-1/LedSwitch") == 0) {
            SetLedSwitch(json_object_get_boolean(val));
          }
          else if(strcmp(key, "LIGHT-1/RedConf") == 0) {
            SetRedConf(json_object_get_int(val));
          }
          else if(strcmp(key, "LIGHT-1/GreenConf") == 0) {
            SetGreenConf(json_object_get_int(val));
          }
          else if(strcmp(key, "LIGHT-1/BlueConf") == 0) {
            SetBlueConf(json_object_get_int(val));
          }
          else if(strcmp(key, "MUSIC-1/Volume") == 0) {
            SetVolume(json_object_get_int(val));
          }
          else if(strcmp(key, "MUSIC-1/Urlpath") == 0) {
            
          }
          else if(strcmp(key, "MUSIC-1/GetTrackList") == 0) {
            
          }
          else if(strcmp(key, "MUSIC-1/DelTrack") == 0) {
            
          }
          else if(strcmp(key, "HEALTH-1/RemindType") == 0) {
            SetRemindType(json_object_get_int(val));
          }

          // for echo debug
          else if(strcmp(key, "DEVICE-1/Power") == 0){
            SetPower(json_object_get_int(val));
          }
          else if(strcmp(key, "DEVICE-1/LowPowerAlarm") == 0) {
            SetLowPowerAlarm(json_object_get_boolean(val));
          }
          else if(strcmp(key, "DEVICE-1/SignalStrength") == 0) {
            SetSignalStrengh(json_object_get_int(val));
          }
          else if(strcmp(key, "DEVICE-1/Temperature") == 0) {
            SetTemperature(json_object_get_double(val));
          }
          else if(strcmp(key, "DEVICE-1/TFStatus") == 0) {
            SetTFStatus(json_object_get_boolean(val));
          }
          else if(strcmp(key, "DEVICE-1/TFCapacity") == 0) {
            SetTFCapacity(json_object_get_double(val));
          }
          else if(strcmp(key, "DEVICE-1/TFFree") == 0) {
            SetTFFree(json_object_get_double(val));
          }
          else if(strcmp(key, "DEVICE-1/DownLoadRate") == 0) {
            SetDownLoadRate(json_object_get_int(val));
          }
          else if(strcmp(key, "DEVICE-1/DrinkStamp") == 0) {
            SetDrinkStamp(json_object_get_int(val));
          }
          else if(strcmp(key, "DEVICE-1/PutDownStamp") == 0) {
            SetPutDownStamp(json_object_get_int(val));
          }
        }
        
        // free memory of json object
        json_object_put(recv_json_object);
        recv_json_object = NULL;
      }
      
      // NOTE: must free msg memory after been used.
      if(NULL != recv_msg){
        free(recv_msg);
        recv_msg = NULL;
      }
    }
  }

exit:
  user_log("ERROR: user_downstream_thread exit with err=%d", err);
}
