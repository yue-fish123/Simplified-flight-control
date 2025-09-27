#ifndef __USER_H
#define __USER_H

#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>                        //va_start /vsnprintf /va_end function
#include <math.h>

//硬件层驱动头文件
#include "stm32g0xx_hal.h"

#include "control.h"
#include "types.h"
#include "i2c.h"

//MPU6050
#include "anbt_dmp_driver.h"
#include "anbt_dmp_mpu6050.h"



void APP_Init(void);

#endif

