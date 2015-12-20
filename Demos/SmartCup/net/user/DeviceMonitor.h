


/***

History:
2015-12-20: Ted: Create

*/

#ifndef _DEVICEMONITOR_H
#define _DEVICEMONITOR_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "Object_int.h"
#include "stdbool.h"
   

void PowerNotification();
void LowPowerAlarmNotification();
void SignalStrengthNotification();
void TFCardNotification();

   
#ifdef __cplusplus
}
#endif

#endif // _DEVICEMONITOR_H

// end of file


