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
#include "MICOAppDefine.h"
#include "MicoFogCloud.h"
#include "json_c/json.h"
#include "If_MO.h"

/* User defined debug log functions
 * Add your own tag like: 'USER', the tag will be added at the beginning of a log
 * in MICO debug uart, when you call this function.
 */
#define user_log(M, ...) custom_log("USER", M, ##__VA_ARGS__)
#define user_log_trace() custom_log_trace("USER")


static mico_thread_t user_downstrem_thread_handle = NULL;
static mico_thread_t user_upstream_thread_handle = NULL;
  
extern void user_downstream_thread(void* arg);
extern void user_upstream_thread(void* arg);


/* user main function, called by AppFramework after system init done && wifi
 * station on in user_main thread.
 */
OSStatus net_main( app_context_t * const app_context )
{
  user_log_trace();
  OSStatus err = kUnknownErr;

  require(app_context, exit);

  MOInit();
 
  // start the downstream thread to handle user command
  err = mico_rtos_create_thread(&user_downstrem_thread_handle, MICO_APPLICATION_PRIORITY, "user_downstream", 
                                user_downstream_thread, STACK_SIZE_USER_DOWNSTREAM_THREAD, 
                                app_context );
  require_noerr_action( err, exit, user_log("ERROR: create user_downstream thread failed!") );
    
  // start the upstream thread to upload temperature && humidity to user
  err = mico_rtos_create_thread(&user_upstream_thread_handle, MICO_APPLICATION_PRIORITY, "user_upstream", 
                                  user_upstream_thread, STACK_SIZE_USER_UPSTREAM_THREAD, 
                                  app_context );
  require_noerr_action( err, exit, user_log("ERROR: create user_uptream thread failed!") );
  
  // user_main loop, update oled display every 1s
  //while(1){
  //  mico_thread_msleep(500);
  //}
   return err;
   
exit:
  if(kNoErr != err){
    user_log("ERROR: user_main thread exit with err=%d", err);
  }
  mico_rtos_delete_thread(NULL);  // delete current thread
  return err;
}
