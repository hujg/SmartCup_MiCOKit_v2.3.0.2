
/***

History:
2015-09-11: Ted: Create

*/

#ifndef _OBJMUSIC_H
#define _OBJMUSIC_H

#ifdef __cplusplus
 extern "C" {
#endif 


#include <Object_int.h>


void ObjMusicInit();
OSStatus GetTrackString(char* str);
OSStatus SetTrackString(const char* str);
OSStatus GetUrlPathString(char* str);
OSStatus SetUrlPathString(const char* str);
OSStatus GetVolumeValue(u16* value);
OSStatus SetVolumeValue(u16 value);


#ifdef __cplusplus
}
#endif

#endif // _OBJMUSIC_H

// end of file