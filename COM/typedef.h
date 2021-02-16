#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#define true            1
#define false           0

typedef char            int8_t;
typedef unsigned char   uint8_t;

typedef short           int16_t;
typedef unsigned short  uint16_t;

typedef int             int32_t;
typedef unsigned int    uint32_t;

typedef uint8_t             bool;

#define MIN(x, y)   (x > y ? y : x)
#define MAX(x, y)   (x > y ? x : y)

#endif
