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
 * Add your own tag like: 'USER_DOWNSTREAM', the tag will be added at the beginning of a log
 * in MICO debug uart, when you call this function.
 */
#define user_log(M, ...) custom_log("USER_DOWNSTREAM", M, ##__VA_ARGS__)
#define user_log_trace() custom_log_trace("USER_DOWNSTREAM")

//
extern bool subscribe;
extern char* track;
extern char* url_path;

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
  
  user_log("user_downstream_thread: Start");
  
  /* thread loop to handle cloud message */
  while(1){
    mico_thread_sleep(1);
    if(!MiCOFogCloudIsActivated(app_context)) {
        user_log("appStatus.fogcloudStatus.isCloudConnected = false");
        //mico_thread_sleep(1);
        continue;
    }

    // recv_msg->data = <topic><data>
    err = MiCOFogCloudMsgRecv(app_context, &recv_msg, 1000);
    user_log("err = %d", err);
    if(kNoErr == err){
        user_log("Msg recv: topic[%d]=[%.*s]\tdata[%d]=[%.*s]", 
                recv_msg->topic_len, recv_msg->topic_len, recv_msg->data, 
                recv_msg->data_len, recv_msg->data_len, recv_msg->data + recv_msg->topic_len);
        
        recv_json_object = json_tokener_parse((const char*)recv_msg->data + recv_msg->topic_len);
        if(NULL != recv_json_object) {
            user_log("recv_json_object != NULL");
            
            // parse json object
            char *key; struct json_object *val; struct lh_entry *entry;

            for(entry = json_object_get_object(recv_json_object)->head; \
                    (entry ? (key = (char*)entry->k, val = (struct json_object*)entry->v, entry) : 0); \
                    entry = entry->next) {
                if(!strcmp(key, "energy")){
                    SetEnergyValue(json_object_get_int(val));
                }
                else if(!strcmp(key, "interval")) {
                    SetIntervalValue(json_object_get_int(val));
                }
                else if(!strcmp(key, "lights")) {
                    SetLightsValue(json_object_get_int(val));
                }
                else if(!strcmp(key, "remind")) {
                    SetRemindValue(json_object_get_int(val));
                }
                else if(!strcmp(key, "volume")) {
                    SetVolumeValue(json_object_get_int(val));
                }
                else if(!strcmp(key, "subscribe")) {
                    subscribe = json_object_get_boolean(val);
                    user_log("subscribe = %d", (int)subscribe);
                }
                else if(!strcmp(key, "track")) {
                    SetTrackString(json_object_get_string(val));
                }
                else if(!strcmp(key, "url_path")) {
                    SetUrlPathString(json_object_get_string(val));
                }
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

exit:
  user_log("ERROR: user_downstream_thread exit with err=%d", err);
}
