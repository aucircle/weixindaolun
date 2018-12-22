/**
  ************************************************************
  ************************************************************
  ************************************************************
  *   文件名：   net_io.cpp
  *
  *   作者：     张继瑞
  *
  *   日期：     2016-11-30
  *
  *   版本：     V1.0
  *
  *   说明：     网络设备数据IO层
  *
  *   修改记录： 
  ************************************************************
  ************************************************************
  ************************************************************
**/

//Arduino头文件
#include <Arduino.h>

//硬件驱动
#include "net_io.h"

/*
************************************************************
*   函数名称： NET_IO_Init
*
*   函数功能： 初始化网络设备IO驱动层
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：   底层的数据收发驱动
************************************************************
*/
void NET_IO_CLASS::NET_IO_Init(void)
{

    Serial.begin(115200);

}

/*
************************************************************
*   函数名称： NET_IO_Send
*
*   函数功能： 发送数据
*
*   入口参数： str：需要发送的数据
*              len：数据长度
*
*   返回参数： 无
*
*   说明：   底层的数据发送驱动
*
************************************************************
*/
void NET_IO_CLASS::NET_IO_Send(unsigned char *str, unsigned short len)
{

    Serial.write(str, len);
    Serial.flush();

}

/*
************************************************************
*   函数名称： NET_IO_WaitRecive
*
*   函数功能： 等待接收完成
*
*   入口参数： 无
*
*   返回参数： REV_OK-接收完成   REV_WAIT-接收超时未完成
*
*   说明：   循环调用检测是否接收完成
************************************************************
*/
bool NET_IO_CLASS::NET_IO_WaitRecive(void)
{

    unsigned char count = 0;
        
    NET_IO_CLASS::dataLen = Serial.available();
    if(NET_IO_CLASS::dataLen == 0)
        return REV_WAIT;
        
    if(NET_IO_CLASS::dataLen == NET_IO_CLASS::dataLenPre)
    {
          for(; count < NET_IO_CLASS::dataLen; count++)
              NET_IO_CLASS::buf[count] = Serial.read();
              
          NET_IO_CLASS::dataLen = 0;
          return REV_OK;
    }

    NET_IO_CLASS::dataLenPre = NET_IO_CLASS::dataLen;

    return REV_WAIT;

}

/*
************************************************************
*   函数名称： NET_IO_ClearRecive
*
*   函数功能： 清空缓存
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：     
************************************************************
*/
void NET_IO_CLASS::NET_IO_ClearRecive(void)
{

    NET_IO_CLASS::dataLen = 0;
    memset(NET_IO_CLASS::buf, 0, sizeof(NET_IO_CLASS::buf));

}
