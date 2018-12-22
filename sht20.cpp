/**
  ************************************************************
  ************************************************************
  ************************************************************
  *   文件名：   sht20.cpp
  *
  *   作者：     张继瑞
  *
  *   日期：     2016-11-30
  *
  *   版本：     V1.0
  *
  *   说明：     温湿度读取
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
#include "sht20.h"
#include "i2c.h"

static I2C_CLASS I2C;
static const int16_t POLYNOMIAL = 0x131;

/*
************************************************************
*   函数名称： SHT20_reset
*
*   函数功能： SHT20复位
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：   
************************************************************
*/
void SHT20_reset(void)
{

    I2C.I2C_WriteByte(SHT20_ADDRESS, SHT20_SOFT_RESET, NULL);

}

/*
************************************************************
*   函数名称： SHT20_read_user_reg
*
*   函数功能： SHT20读取用户寄存器
*
*   入口参数： 无
*
*   返回参数： 读取到的用户寄存器的值
*
*   说明：   
************************************************************
*/
unsigned char  SHT20_read_user_reg(void)
{

    unsigned char val = 0;
  
    I2C.I2C_ReadByte(SHT20_ADDRESS, SHT20_READ_REG, &val);
  
    return val;

}

/*
************************************************************
*   函数名称： SHT2x_CheckCrc
*
*   函数功能： 检查数据正确性
*
*   入口参数： data：读取到的数据
*              nbrOfBytes：需要校验的数量
*              checksum：读取到的校对比验值
*
*   返回参数： 校验结果
*
*   说明：   0-成功    1-失败
************************************************************
*/
char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum)
{

    char crc = 0;
    char _bit = 0;
    char byteCtr = 0;
  
    //calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( _bit = 8; _bit > 0; --_bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }
  
    if(crc != checksum)
        return 1;
    else
        return 0;

}

/*
************************************************************
*   函数名称： SHT2x_CalcTemperatureC
*
*   函数功能： 温度计算
*
*   入口参数： u16sT：读取到的温度原始数据
*
*   返回参数： 计算后的温度数据
*
*   说明：   
************************************************************
*/
float SHT2x_CalcTemperatureC(unsigned short u16sT)
{
  
    float temperatureC = 0;            // variable for result

    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [癈] --
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; //T= -46.85 + 175.72 * ST/2^16
  
    return temperatureC;
  
}

/*
************************************************************
*   函数名称： SHT2x_CalcRH
*
*   函数功能： 湿度计算
*
*   入口参数： u16sRH：读取到的湿度原始数据
*
*   返回参数： 计算后的湿度数据
*
*   说明：   
************************************************************
*/
float SHT2x_CalcRH(unsigned short u16sRH)
{
  
    float humidityRH = 0;              // variable for result
  
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    //humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
    humidityRH = ((float)u16sRH * 0.00190735) - 6;
  
    return humidityRH;
  
}

/*
************************************************************
*   函数名称： SHT2x_MeasureHM
*
*   函数功能： 测量温湿度
*
*   入口参数： cmd：测量温度还是湿度
*              pMeasurand：不为空则保存为ushort值到此地址
*
*   返回参数： 测量结果
*
*   说明：   
************************************************************
*/
float SHT2x_MeasureHM(unsigned char cmd, unsigned short *pMeasurand)
{
  
    char  data[3];    //data array for checksum verification
    unsigned short tmp = 0;
    float t = 0;
    unsigned char count = 0;
  
    Wire.beginTransmission(SHT20_ADDRESS);

    Wire.write(cmd);

    Wire.endTransmission();

    Wire.requestFrom(SHT20_ADDRESS, 3);
    
    delay(70);
    
    if(Wire.available() >= 3)
    {
        for(; count < 3; count++)
        {
            data[count] = Wire.read();
        }
    }
    
    SHT2x_CheckCrc(data, 2, data[2]);
    tmp = (data[0] << 8) + data[1];
    if(cmd == SHT20_Measurement_T_HM)
    {
        t = SHT2x_CalcTemperatureC(tmp);
    }
    else
    {
        t = SHT2x_CalcRH(tmp);
    }
  
    if(pMeasurand)
    {
        *pMeasurand = (unsigned short)t;
    }
  
    return t;
  
}

/*
************************************************************
*   函数名称： SHT20_GetValue
*
*   函数功能： 获取温湿度数据
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：   温湿度结果保存在SHT20结构体里
************************************************************
*/
void SHT20_CLASS::SHT20_GetValue(void)
{
  
    unsigned char val = 0;
    
    SHT20_read_user_reg();
    delayMicroseconds(100);
    
    SHT20_CLASS::tempreture = (unsigned char)SHT2x_MeasureHM(SHT20_Measurement_T_HM, NULL);
    delay(70);
    
    SHT20_CLASS::humidity = (unsigned char)SHT2x_MeasureHM(SHT20_Measurement_RH_HM, NULL);
    delay(25);
    
    SHT20_read_user_reg();
    delay(25);
    
    I2C.I2C_WriteByte(SHT20_ADDRESS, SHT20_WRITE_REG, &val);
    delayMicroseconds(100);
    
    SHT20_read_user_reg();
    delayMicroseconds(100);
    
    SHT20_reset();
    delayMicroseconds(100);

}

