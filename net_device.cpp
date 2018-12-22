/**
  ************************************************************
  ************************************************************
  ************************************************************
  *   文件名：   net_device.cpp
  *
  *   作者：     张继瑞
  *
  *   日期：     2016-11-30
  *
  *   版本：     V1.0
  *
  *   说明：     网络设备应用层
  *
  *   修改记录： 
  ************************************************************
  ************************************************************
  ************************************************************
**/

//Arduino头文件
#include <Arduino.h>

//硬件驱动
#include "net_device.h"
#include "net_io.h"
#include "led.h"

static NET_IO_CLASS NET_IO;
extern LED_CLASS LED;

/*
************************************************************
*   函数名称： NET_DEVICE_IO_Init
*
*   函数功能： 初始化网络设备IO层
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：   初始化网络设备的控制引脚、数据收发功能等
************************************************************
*/
void NET_DEVICE_CLASS::NET_DEVICE_IO_Init(void)
{

    NET_IO.NET_IO_Init();
    pinMode(2, OUTPUT);

}

/*
************************************************************
*   函数名称： NET_DEVICE_Init
*
*   函数功能： 网络设备初始化
*
*   入口参数： 无
*
*   返回参数： 返回初始化结果
*
*   说明：   0-成功    1-失败
************************************************************
*/
bool NET_DEVICE_CLASS::NET_DEVICE_Init(void)
{

    unsigned char errCount = 0, errType = 0;
    char cfgBuffer[70];

    switch(NET_DEVICE_CLASS::initStep)
    {
        case 0:
            if(!NET_DEVICE_CLASS::NET_DEVICE_SendCmd("AT+CWMODE=1\r\n", "OK"))
                NET_DEVICE_CLASS::initStep++;
        break;

        case 1:
            if(!NET_DEVICE_CLASS::NET_DEVICE_SendCmd("AT+CIPMODE=0\r\n", "OK"))
                NET_DEVICE_CLASS::initStep++;
        break;

        case 2:
        
            memset(cfgBuffer, 0, sizeof(cfgBuffer));
            strcpy(cfgBuffer, "AT+CWJAP=\"");                           //填写ssid、pswd
            strcat(cfgBuffer, staName);
            strcat(cfgBuffer, "\",\"");
            strcat(cfgBuffer, staPass);
            strcat(cfgBuffer, "\"\r\n");
          
            while(NET_DEVICE_SendCmd(cfgBuffer, "GOT IP"))                      //连接路由器，检索“GOT IP”，如果失败会进入循环体
            {
                LED.Led1_Set(LED_ON);                                 //亮灯提示
                delay(500);
                
                LED.Led1_Set(LED_OFF);                                  //灭灯提示
                delay(500);
                
                if(++errCount >= 5)                                 //如果连续5次没有连上，需要检测网络状态或者ssid、pswd
                {
                    errType = 1; //wifi信息错误
                    break;
                }
            }
      
            if(!errType)
                NET_DEVICE_CLASS::initStep++;
              
        break;

        case 3:
        
            while(NET_DEVICE_SendCmd("AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n", "CONNECT"))  //连接平台，检索“CONNECT”，如果失败会进入循环体
            {
                LED.Led2_Set(LED_ON);
                delay(100);
                
                LED.Led2_Set(LED_OFF);
                delay(100);
                
                if(++errCount >= 10)
                {
                    errType = 2; //设备id或者apikey错误
                    break;
                }
            }
      
            if(!errType)
            {
                NET_DEVICE_CLASS::NET_DEVICE_ClrData();
                NET_DEVICE_CLASS::initStep++;
            }
            
        break;
    }

    if(NET_DEVICE_CLASS::initStep == 4)
        return 0;
    else
        return 1;

}

/*
************************************************************
*   函数名称： NET_DEVICE_Reset
*
*   函数功能： 网络设备复位
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：   
************************************************************
*/
void NET_DEVICE_CLASS::NET_DEVICE_Reset(void)
{

    digitalWrite(2, LOW);
    delay(200);

    digitalWrite(2, HIGH);
    delay(500);

}

/*
************************************************************
*   函数名称： NET_DEVICE_ReLink
*
*   函数功能： 重连平台
*
*   入口参数： 无
*
*   返回参数： 返回连接结果
*
*   说明：   0-成功    1-失败
************************************************************
*/
bool NET_DEVICE_CLASS::NET_DEVICE_ReLink(void)
{
    bool status = 0;

    NET_DEVICE_CLASS::NET_DEVICE_SendCmd("AT+CIPCLOSE\r\n", "OK");                        //连接前先关闭一次
    delay(200);                                      //等待
    status = NET_DEVICE_CLASS::NET_DEVICE_SendCmd("AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n", "CONNECT");  //重新连接

    return status;

}

/*
************************************************************
*   函数名称： NET_DEVICE_SendCmd
*
*   函数功能： 向网络设备发送一条命令，并等待正确的响应
*
*   入口参数： cmd：需要发送的命令   res：需要检索的响应
*
*   返回参数： 返回连接结果
*
*   说明：   0-成功    1-失败
************************************************************
*/
bool NET_DEVICE_CLASS::NET_DEVICE_SendCmd(char *cmd, char *res)
{

    unsigned char timeOut = 200;

    NET_IO.NET_IO_Send((unsigned char *)cmd, strlen(cmd));

    while(timeOut--)
    {
        if(NET_IO.NET_IO_WaitRecive() == REV_OK)              //如果收到数据
        {
            if(strstr((const char *)NET_IO.buf, res) != NULL)  //如果检索到关键词
            {
                NET_IO.NET_IO_ClearRecive();               //清空缓存
                return 0;
            }
        }
        delay(10);
    }

    return 1;

}

/*
************************************************************
*   函数名称： NET_DEVICE_SendData
*
*   函数功能： 使网络设备发送数据到平台
*
*   入口参数： data：需要发送的数据
*              len：数据长度
*
*   返回参数： 无
*
*   说明：   
************************************************************
*/
void NET_DEVICE_CLASS::NET_DEVICE_SendData(unsigned char *data, unsigned short len)
{

    char cmdBuf[30];

    delay(50);
    NET_IO.NET_IO_ClearRecive();              //清空接收缓存
    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);    //发送命令
    //if(!NET_DEVICE_SendCmd(cmdBuf, ">"))      //收到‘>’时可以发送数据
    NET_DEVICE_SendCmd(cmdBuf, ">");
    {
        NET_IO.NET_IO_Send(data, len);           //发送设备连接请求数据
    }

}

/*
************************************************************
*   函数名称： NET_DEVICE_GetIPD
*
*   函数功能： 获取平台返回的数据
*
*   入口参数： 等待的时间(乘以10ms)
*
*   返回参数： 平台返回的原始数据
*
*   说明：     不同网络设备返回的格式不同，需要去调试
               如ESP8266的返回格式为  "+IPD,x:yyy"  x代表数据长度，yyy是数据内容
************************************************************
*/
unsigned char *NET_DEVICE_CLASS::NET_DEVICE_GetIPD(unsigned short timeOut)
{

    unsigned char byte = 0, count = 0;
    char sByte[5];
    char *ptrIPD;

    do
    {
        if(NET_IO.NET_IO_WaitRecive() == REV_OK)               //如果接收完成
        {
            ptrIPD = strstr((char *)NET_IO.buf, "IPD,");       //搜索“IPD”头
            if(ptrIPD == NULL)                      //如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
            {
                //UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
            }
            else
            {
                ptrIPD = strstr(ptrIPD, ",");ptrIPD++;          //找到','  ，然后偏移到下一个字符，代表数据长度的第一个数字
                
                while(*ptrIPD != ':')                 //在','和':' 之间的都是数据长度的数据
                {
                    sByte[count++] = *ptrIPD++;
                }
                byte = (unsigned char)atoi(sByte);            //将字符转为数值形式
                
                ptrIPD++;                       //此时ptrIPD指针还指向':'，所以需要偏移到下个数据
                for(count = 0; count < byte; count++)         //复制数据
                {
                    NET_IO.buf[count] = *ptrIPD++;
                }
                return NET_IO.buf;                 //复制完成，返回数据指针
            }
        }
        delay(10);                        //延时等待
    } while(timeOut--);
  
  return NULL;                            //超时还未找到，返回空指针

}

/*
************************************************************
*   函数名称： NET_DEVICE_ClrData
*
*   函数功能： 清空网络设备数据接收缓存
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：   
************************************************************
*/
void NET_DEVICE_CLASS::NET_DEVICE_ClrData(void)
{

    NET_IO.NET_IO_ClearRecive();

}

/*
************************************************************
*   函数名称： NET_DEVICE_Check
*
*   函数功能： 检查网络设备连接状态
*
*   入口参数： 无
*
*   返回参数： 返回状态
*
*   说明：   
************************************************************
*/
unsigned char NET_DEVICE_CLASS::NET_DEVICE_Check(void)
{

    unsigned char status = 0;
    unsigned char timeOut = 200;

    NET_IO.NET_IO_ClearRecive();                        //清空缓存
    NET_IO.NET_IO_Send((unsigned char *)"AT+CIPSTATUS\r\n",  14);        //发送状态监测
    
    while(--timeOut)
    {
        if(NET_IO.NET_IO_WaitRecive() == REV_OK)
        {
            if(strstr((const char *)NET_IO.buf, "STATUS:2"))     //获得IP
            {
                status = 2;
            }
            else if(strstr((const char *)NET_IO.buf, "STATUS:3"))  //建立连接
            {
                status = 0;
            }
            else if(strstr((const char *)NET_IO.buf, "STATUS:4"))  //失去连接
            {
                status = 1;
            }
            else if(strstr((const char *)NET_IO.buf, "STATUS:5"))  //物理掉线
            {
                status = 3;
            }
            
            break;
        }
        
        delay(10);
    }
    
    if(timeOut == 0)
    {
        status = 3;
    }
    
    return status;

}

/*
************************************************************
*   函数名称： NET_DEVICE_ReConfig
*
*   函数功能： 设备网络设备初始化的步骤
*
*   入口参数： 步骤值
*
*   返回参数： 无
*
*   说明：    该函数设置的参数在网络设备初始化里边用到
************************************************************
*/
void NET_DEVICE_CLASS::NET_DEVICE_ReConfig(unsigned char step)
{

    NET_DEVICE_CLASS::initStep = step;

}

/*
************************************************************
*   函数名称： NET_DEVICE_Set_DataMode
*
*   函数功能： 设置设备数据收发模式
*
*   入口参数： 模式
*
*   返回参数： 无
*
*   说明：   
************************************************************
*/
void NET_DEVICE_CLASS::NET_DEVICE_Set_DataMode(unsigned char mode)
{

    NET_DEVICE_CLASS::dataType = mode;

}

/*
************************************************************
*   函数名称： NET_DEVICE_Get_DataMode
*
*   函数功能： 获取设备数据收发模式
*
*   入口参数： 无
*
*   返回参数： 模式
*
*   说明：   
************************************************************
*/
unsigned char NET_DEVICE_CLASS::NET_DEVICE_Get_DataMode(void)
{

    return NET_DEVICE_CLASS::dataType;

}
