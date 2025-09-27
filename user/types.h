/**
******************************************************************************
* @file         types.h
* @version      V1.0
* @date         2020-06-05
* @brief        常用类型重定义
*
* @company      深圳炜世科技有限公司
* @information  WIZnet W5500官方代理商，全程技术支持，价格优势大！
* @website      www.wisioe.com
* @forum        www.w5500.cn
* @qqGroup      579842114
* @Support      QQ:2571856470；Mob:18998931307
* @Email        support@wisioe.com
******************************************************************************
*/
#ifndef _TYPE_H_
#define _TYPE_H_
#include "user.h"


#define MAX_SOCK_NUM 8 // Maxmium number of socket

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

//typedef unsigned char unsigned char;
typedef unsigned short u16;
//typedef unsigned int unsigned int;

typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int vu32;


//typedef int int16;

typedef uint8_t  SOCKET;


#endif /* _TYPE_H_ */
