/**
  ************************************************************
  ************************************************************
  ************************************************************
  * 文件名：  fault.c
  *
  * 作者：     张继瑞
  *
  * 日期：     2016-11-29
  *
  * 版本：     V1.0
  *
  * 说明：     网络状态错误处理
  *
  * 修改记录： 
  ************************************************************
  ************************************************************
  ************************************************************
**/

#include "fault.h"

#include "onenet.h"
#include "net_device.h"

extern ONENET_CLASS ONENET;
extern NET_DEVICE_CLASS NET_DEVICE;

/*
************************************************************
* 函数名称： Fault_Process
*
* 函数功能： 网络状态错误处理
*
* 入口参数： 无
*
* 返回参数： 无
*
* 说明：   针对死机重启、协议错误、设备错误的处理
************************************************************
*/
void FAULT_CLASS::Fault_Process(void)
{

    NET_DEVICE.NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);           //设置为命令收发模式
    
    switch(faultType)
    {
        case FAULT_REBOOT:                      //死机重启的处理
          
            FAULT_CLASS::faultType = FAULT_NONE;                 //死机重启后，设备会被复位，所以是重新初始化
        
        break;
        
        case FAULT_EDP: //协议出错的处理
          
            FAULT_CLASS::faultType = FAULT_NONE;
            
            NET_DEVICE.NET_DEVICE_ReLink();                  //连接IP
            ONENET.netWork = 0;                 //这里设置重连平台
        
        break;
        
        case FAULT_NODEVICE:                    //设备出错的处理
          
            FAULT_CLASS::faultType = FAULT_NONE;
            
            NET_DEVICE.NET_DEVICE_Reset();                   //网络设备复位
            NET_DEVICE.NET_DEVICE_ReConfig(0);               //重新初始化
            ONENET.netWork = 0;                 //网络未连接
        
        break;
        
        default:
            break;
    }
    
    NET_DEVICE.NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);            //设置为数据收发模式

}

