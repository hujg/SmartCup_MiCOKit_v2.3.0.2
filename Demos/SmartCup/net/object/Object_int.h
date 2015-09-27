
/***

History:
2015-09-11: Ted: Create

*/

#ifndef _OBJECT_INT_H
#define _OBJECT_INT_H
#include "MICO.h"
#ifdef __cplusplus
 extern "C" {
#endif 

#if 0
typedef signed char     i8;
typedef unsigned char   u8;
typedef signed int      i16;
typedef unsigned int    u16;
typedef signed long     i32;
typedef unsigned long   u32;
#else
#define i8     int8_t
#define u8     uint8_t
#define i16    int16_t
#define u16    uint16_t
#define i32    int32_t
#define u32    uint32_t
#endif

#ifdef __cplusplus
}
#endif

#endif // _OBJECT_INT_H

// end of file