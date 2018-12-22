/**
  ************************************************************
  ************************************************************
  ************************************************************
  *   文件名：   i2c.cpp
  *
  *   作者：     张继瑞
  *
  *   日期：     2016-11-30
  *
  *   版本：     V1.0
  *
  *   说明：     i2c初始化、总线控制
  *
  *   修改记录： 
  ************************************************************
  ************************************************************
  ************************************************************
**/

//Arduino头文件
#include <Arduino.h>
#include <Wire.h>

//硬件驱动
#include "i2c.h"

/*
************************************************************
*   函数名称： I2C_Init
*
*   函数功能： i2c总线初始化
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：     
************************************************************
*/
void I2C_CLASS::I2C_Init(void)
{
  
    Wire.begin();
  
}

/*
************************************************************
*   函数名称： I2C_WriteByte
*
*   函数功能： 写一个字节
*
*   入口参数： slaveAddr：设备地址
*              regAddr：寄存器地址
*              data：数值指针
*
*   返回参数： 0-成功    1-失败
*
*   说明：     未判断成功与失败，有待添加
************************************************************
*/
bool I2C_CLASS::I2C_WriteByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *data)
{

      Wire.beginTransmission(slaveAddr);
      
      Wire.write(regAddr);
      
      if(data != NULL)
          Wire.write(*data);
      
      Wire.endTransmission();

      return 0;

}

/*
************************************************************
*   函数名称： I2C_ReadByte
*
*   函数功能： 读一个字节
*
*   入口参数： slaveAddr：设备地址
*              regAddr：寄存器地址
*              val：数值指针
*
*   返回参数： 0-成功    1-失败
*
*   说明：     未判断成功与失败，有待添加
************************************************************
*/
bool I2C_CLASS::I2C_ReadByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *val)
{

    Wire.beginTransmission(slaveAddr);

    Wire.write(regAddr);

    Wire.endTransmission();

    Wire.requestFrom(slaveAddr, 1);
    if(Wire.available() >= 1)
    {
        *val = Wire.read();
    }

    return 0;

}

/*
************************************************************
*   函数名称： I2C_WriteBytes
*
*   函数功能： 写多个字节
*
*   入口参数： slaveAddr：设备地址
*              regAddr：寄存器地址
*              buf：数据指针
*              num：数据长度
*
*   返回参数： 0-成功    1-失败
*
*   说明：     未判断成功与失败，有待添加
************************************************************
*/
bool I2C_CLASS::I2C_WriteBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num)
{

    unsigned char count = 0;

    Wire.beginTransmission(slaveAddr);
      
    Wire.write(regAddr);

    for(; count < num; count++)
    {
        Wire.write(buf[count]);
    }

    Wire.endTransmission();

    return 0;

}

/*
************************************************************
*   函数名称： I2C_ReadBytes
*
*   函数功能： 读多个字节
*
*   入口参数： slaveAddr：设备地址
*              regAddr：寄存器地址
*              buf：数据指针
*              num：数据长度
*
*   返回参数： 0-成功    1-失败
*
*   说明：     未判断成功与失败，有待添加
************************************************************
*/
bool I2C_CLASS::I2C_ReadBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num)
{

    unsigned char count = 0;

    Wire.beginTransmission(slaveAddr);

    Wire.write(regAddr);

    Wire.endTransmission();

    Wire.requestFrom(slaveAddr, num);

    if(Wire.available() >= num)
    {
        for(; count < num; count++)
        {
            buf[count] = Wire.read();
        }
    }

    return 0;

}
