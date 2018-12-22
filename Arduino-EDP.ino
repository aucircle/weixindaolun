/**
  ************************************************************
  ************************************************************
  ************************************************************
  *   文件名：    
  *
  *   作者：     张继瑞
  *
  *   日期：     2016-11-30
  *
  *   版本：     V1.0
  *
  *   说明：     完成单片机初始化、外接IC初始化
  *
  *   修改记录： 
  ************************************************************
  ************************************************************
  ************************************************************
**/

//协议
#include "onenet.h"
#include "fault.h"

//网络设备
#include "net_device.h"

//硬件驱动
#include "led.h"
#include "beep.h"
#include "i2c.h"
#include "sht20.h"
#include "light.h"
#include "key.h"

NET_DEVICE_CLASS NET_DEVICE;
ONENET_CLASS ONENET;
FAULT_CLASS FAULT;

LED_CLASS LED;
BEEP_CLASS BEEP;
I2C_CLASS I2C;
LIGHT_CLASS LIGHT;
KEY_CLASS KEY;
SHT20_CLASS SHT20;

unsigned long runTime = 0;
bool sendFlag = 1;
unsigned char *dataPtr;

/*
************************************************************
*   函数名称： setup
*
*   函数功能： 硬件初始化
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：     
************************************************************
*/
void setup()
{

    LED.Led_Init();
    BEEP.Beep_Init();
    I2C.I2C_Init();
    LIGHT.Light_Init();
    KEY.KEY_Init();

    NET_DEVICE.NET_DEVICE_IO_Init();
    NET_DEVICE.NET_DEVICE_Reset();
    NET_DEVICE.NET_DEVICE_Set_DataMode(0);
    

}

/*
************************************************************
*   函数名称： loop
*
*   函数功能： Super-Loop
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：     
************************************************************
*/
void loop()
{

    if(ONENET.netWork)
    {
        /****************************************************************************
          定时上传数据 和 心跳请求
        ****************************************************************************/
        if(millis() - runTime >= 15000)                 //如果运行时间离上次数据发送超过了15s
        {
            runTime = millis();
            if(sendFlag)
            {
                SHT20.SHT20_GetValue();
                LIGHT.Light_GetValue();
                ONENET.OneNet_SendData();               //上传数据
            }
            else
                ONENET.OneNet_HeartBeat();              //心跳连接
    
            sendFlag = !sendFlag;
        }

        /****************************************************************************
          错误处理
        ****************************************************************************/
        if(FAULT.faultType != FAULT_NONE)                  //如果错误标志被设置
        {
            FAULT.Fault_Process();                        //进入错误处理函数
        }

        /****************************************************************************
          按键功能
        ****************************************************************************/
        switch(KEY.KEY_Keyboard())
        {
            case KEY1DOWN:
            
                if(LED.led1Sta == LED_ON)
                    LED.Led1_Set(LED_OFF);
                else
                    LED.Led1_Set(LED_ON);

                runTime = millis();
                ONENET.OneNet_SendData();
                    
            break;

            case KEY2DOWN:
            
                if(LED.led2Sta == LED_ON)
                    LED.Led2_Set(LED_OFF);
                else
                    LED.Led2_Set(LED_ON);

                runTime = millis();
                ONENET.OneNet_SendData();
                    
            break;
        }

        /****************************************************************************
          命令解析和处理
        ****************************************************************************/
        dataPtr = NET_DEVICE.NET_DEVICE_GetIPD(0);                    //不等待，获取平台下发的数据
        if(dataPtr != NULL)                                           //如果数据指针不为空，则代表收到了数据
        {
            if(ONENET.OneNet_App(dataPtr))                            //集中处理
            {
                runTime = millis();
                ONENET.sendData = 0;
                ONENET.OneNet_SendData();
            }
            NET_DEVICE.NET_DEVICE_ClrData();
        }
    }
    else
    {
        /****************************************************************************
          网络连接 平台连接
        ****************************************************************************/
        if(!NET_DEVICE.NET_DEVICE_Init())
            ONENET.OneNet_DevLink(ONENET.devID, ONENET.apiKey);
        
        delay(100);
        runTime = millis();
    }

}

