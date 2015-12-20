


/***

History:
2015-12-17: Ted: Create

*/


#include "OMFactory.h"
#include "If_MO.h"
#include "mico.h"
#include "json_c/json.h"


#define user_log(M, ...) custom_log("OMFactory", M, ##__VA_ARGS__)
#define user_log_trace() custom_log_trace("OMFactory")


static void OMFactoryRead();


void OMFactoryInit()
{
    OMFactoryRead();

    // TODO: get Music track

    // TODO: get device info.
}

static void OMFactoryRead()
{
//    char* string;
    json_object *get_json_object = NULL;
    
    // TODO: get string from flash interface

    // parse json data from the msg, get led control value
    //get_json_object = json_tokener_parse(/*string*/);
    if (NULL != get_json_object){
        json_object_object_foreach(get_json_object, key, val) {
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
            else if(strcmp(key, "HEALTH-1/RemindType") == 0) {
                SetRemindType(json_object_get_int(val));
            }
            else if(strcmp(key, "HEALTH-1/RemindTimes") == 0) {
                SetRemindTimes(json_object_get_int(val));
            }
            else if(strcmp(key, "HEALTH-1/IfNoDisturbing") == 0) {
                SetIfNoDisturbing(json_object_get_boolean(val));
            }
            else if(strcmp(key, "HEALTH-1/NoDisturbingStartHour") == 0) {
                SetNoDisturbingStartHour(json_object_get_int(val));
            }
            else if(strcmp(key, "HEALTH-1/NoDisturbingEndHour") == 0) {
                SetNoDisturbingEndHour(json_object_get_int(val));
            }
            else if(strcmp(key, "HEALTH-1/NoDisturbingStartMinute") == 0) {
                SetNoDisturbingStartMinute(json_object_get_int(val));
            }
            else if(strcmp(key, "HEALTH-1/NoDisturbingEndMinute") == 0) {
                SetNoDisturbingEndMinute(json_object_get_int(val));
            }
        }

        // free memory of json object
        json_object_put(get_json_object);
        get_json_object = NULL;
    }
    else {
        user_log("[ERR]parse json object error!");
    }
}

void OMFactorySave()
{
    json_object *save_json_object = NULL;
    const char *save_data = NULL;

    save_json_object = json_object_new_object();
    if(NULL == save_json_object){
        user_log("create json object error!");
    }
    else {
        json_object_object_add(save_json_object, "DEVICE-1/TempSwitch", json_object_new_boolean(GetTempSwitch()));
        json_object_object_add(save_json_object, "DEVICE-1/PowerOnDisplay", json_object_new_boolean(GetPowerOnDisplay()));
        json_object_object_add(save_json_object, "LIGHT-1/LightsType", json_object_new_boolean(GetLightsType()));
        json_object_object_add(save_json_object, "LIGHT-1/EnableNotifyLight", json_object_new_boolean(GetEnableNotifyLight()));
        json_object_object_add(save_json_object, "LIGHT-1/LedSwitch", json_object_new_boolean(GetLedSwitch()));
        json_object_object_add(save_json_object, "LIGHT-1/RedConf", json_object_new_boolean(GetRedConf()));
        json_object_object_add(save_json_object, "LIGHT-1/GreenConf", json_object_new_boolean(GetGreenConf()));
        json_object_object_add(save_json_object, "LIGHT-1/BuleConf", json_object_new_boolean(GetBlueConf()));
        json_object_object_add(save_json_object, "MUSIC-1/Volume", json_object_new_boolean(GetVolume()));
        json_object_object_add(save_json_object, "HEALTH-1/RemindType", json_object_new_boolean(GetRemindType()));
        json_object_object_add(save_json_object, "HEALTH-1/RemindTimes", json_object_new_boolean(GetRemindTimes()));
        json_object_object_add(save_json_object, "HEALTH-1/IfNoDisturbing", json_object_new_boolean(GetIfNoDisturbing()));
        json_object_object_add(save_json_object, "HEALTH-1/NoDisturbingStartHour", json_object_new_boolean(GetNoDisturbingStartHour()));
        json_object_object_add(save_json_object, "HEALTH-1/NoDisturbingStartMinute", json_object_new_boolean(GetNoDisturbingStartMinute()));
        json_object_object_add(save_json_object, "HEALTH-1/NoDisturbingEndHour", json_object_new_boolean(GetNoDisturbingEndHour()));
        json_object_object_add(save_json_object, "HEALTH-1/NoDisturbingEndMinute", json_object_new_boolean(GetNoDisturbingEndMinute()));

        save_data = json_object_to_json_string(save_json_object);
        if(NULL == save_data) {
            user_log("create upload data string error!");
        }
        else {
            // TODO: write flash interface with save_data string
        }
    }

    // free json object memory
    json_object_put(save_json_object);
    save_json_object = NULL;
}



// end of file


