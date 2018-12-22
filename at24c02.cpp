/**
  ************************************************************
  ************************************************************
  ************************************************************
  *   文件名：   at24c02.cpp
  *
  *   作者：     张继瑞
  *
  *   日期：     2016-11-30
  *
  *   版本：     V1.0
  *
  *   说明：     EEPROM操作
  *
  *   修改记录： 
  ************************************************************
  ************************************************************
  ************************************************************
**/

//Arduino头文件
#include <Arduino.h>

//硬件驱动
#include "at24c02.h"
#include "i2c.h"

extern I2C_CLASS I2C;

/*
************************************************************
*   函数名称： AT24C02_WriteByte
*
*   函数功能： 向EEPROM写入一个字节
*
*   入口参数： regAddr：寄存器地址
*              data：数据指针
*
*   返回参数： 0-成功    1-失败
*
*   说明：   
************************************************************
*/
bool AT24C02_CLASS::AT24C02_WriteByte(unsigned char regAddr, unsigned char *data)
{

    return I2C.I2C_WriteByte(AT24C02_ADDRESS, regAddr, data);

}

/*
************************************************************
*   函数名称： AT24C02_ReadByte
*
*   函数功能： 向EEPROM读取一个字节
*
*   入口参数： regAddr：寄存器地址
*              val：数据指针
*
*   返回参数： 0-成功    1-失败
*
*   说明：   
************************************************************
*/
bool AT24C02_CLASS::AT24C02_ReadByte(unsigned char regAddr, unsigned char *val)
{

    return I2C.I2C_ReadByte(AT24C02_ADDRESS, regAddr, val);

}

/*
************************************************************
*   函数名称： AT24C02_WriteBytes
*
*   函数功能： 向EEPROM写入多个字节
*
*   入口参数： regAddr：寄存器地址
*              buf：数据指针
*              num：数据长度
*
*   返回参数： 0-成功    1-失败
*
*   说明：   
************************************************************
*/
bool AT24C02_CLASS::AT24C02_WriteBytes(unsigned char regAddr, unsigned char *buf, unsigned char num)
{

    unsigned char count = 0;

    for(;count < num; count++)
    {
        I2C.I2C_WriteByte(AT24C02_ADDRESS, regAddr, buf);
        regAddr++;
        buf++;
        
        delay(2);
    }

    return 0;

}

/*
************************************************************
*   函数名称： AT24C02_ReadBytes
*
*   函数功能： 向EEPROM读取多个字节
*
*   入口参数： regAddr：寄存器地址
*              buf：数据指针
*              num：数据长度
*
*   返回参数： 0-成功    1-失败
*
*   说明：   
************************************************************
*/
bool AT24C02_CLASS::AT24C02_ReadBytes(unsigned char regAddr, unsigned char *buf, unsigned char num)
{

    return I2C.I2C_ReadBytes(AT24C02_ADDRESS, regAddr, buf, num);

}
