/**
  ************************************************************
  ************************************************************
  ************************************************************
  *   文件名：   onenet.cpp
  *
  *   作者：     张继瑞
  *
  *   日期：     2016-11-30
  *
  *   版本：     V1.0
  *
  *   说明：     网络协议层
  *
  *   修改记录： 
  ************************************************************
  ************************************************************
  ************************************************************
**/

//Arduino头文件
#include <Arduino.h>

//协议层
#include "onenet.h"
#include "fault.h"
#include "net_device.h"
#include "edpkit.h"

//硬件驱动
#include "beep.h"
#include "led.h"
#include "light.h"
#include "sht20.h"

extern NET_DEVICE_CLASS NET_DEVICE;
static EdpPacketClass EDPKIT;
extern FAULT_CLASS FAULT;

extern BEEP_CLASS BEEP;
extern LED_CLASS LED;
extern LIGHT_CLASS LIGHT;
extern SHT20_CLASS SHT20;

/*
************************************************************
*   函数名称： OneNet_DevLink
*
*   函数功能： 与onenet创建连接
*
*   入口参数： devid：创建设备的devid
*              auth_key：创建设备的masterKey或apiKey
*
*   返回参数： 无
*
*   说明：   与onenet平台建立连接，成功或会标记oneNetInfo.netWork网络状态标志
************************************************************
*/
void ONENET_CLASS::OneNet_DevLink(const char* devid, const char* auth_key)
{

    unsigned char *dataPtr;
  
    EDPKIT.PacketConnect1(devid, auth_key);           //根据devid和apikey封装协议包
    
    NET_DEVICE.NET_DEVICE_SendData(EDPKIT.GetData(), EDPKIT.GetWritepos());   //上传平台
    
    dataPtr = NET_DEVICE.NET_DEVICE_GetIPD(250);               //等待平台响应
    if(dataPtr != NULL)
    {
        ONENET_CLASS::netWork = ONENET_CLASS::OneNet_EDPKitCmd(dataPtr);       //解析数据
    }

    EDPKIT.ClearParameter();                 //删包
    
    if(ONENET_CLASS::netWork)                      //如果接入成功
    {
        ONENET_CLASS::errCount = 0;
        
        NET_DEVICE.NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);          //设置为数据收发模式
        
        BEEP.Beep_Set(BEEP_ON);                      //短叫提示成功
        delay(200);
        BEEP.Beep_Set(BEEP_OFF);
    }
    else
    {
        if(++ONENET_CLASS::errCount >= 5)                //如果超过设定次数后，还未接入平台
        {
            ONENET_CLASS::netWork = 0;
            FAULT.faultType = FAULT.faultTypeReport = FAULT_NODEVICE;     //标记为硬件错误
            
            BEEP.Beep_Set(BEEP_ON);                    //长叫提示失败
            delay(500);
            BEEP.Beep_Set(BEEP_OFF);
        }
    }
  
}

/*
************************************************************
*   函数名称： OneNet_EDPKitCmd
*
*   函数功能： EDP协议命令解析
*
*   入口参数： data：平台下发的数据
*
*   返回参数： 无
*
*   说明：     暂时只做了连接解析
************************************************************
*/
_Bool ONENET_CLASS::OneNet_EDPKitCmd(unsigned char *data)
{

    if(data[0] == CONNRESP) //连接响应
    {
        //0   连接成功
        //1   验证失败：协议错误
        //2   验证失败：设备ID鉴权失败
        //3   验证失败：服务器失败
        //4   验证失败：用户ID鉴权失败
        //5   验证失败：未授权
        //6   验证失败：激活码未分配
        //7   验证失败：该设备已被激活
        //8   验证失败：重复发送连接请求包
        //9   验证失败：重复发送连接请求包
        
        if(data[3] == 0)
            return 1;
        else
            return 0;
    }
    
    return 0;

}

/*
************************************************************
*   函数名称： OneNet_SendData
*
*   函数功能： 上传数据到平台
*
*   入口参数： type：发送数据的格式
*
*   返回参数： 无
*
*   说明：     这里需要更改为自己想要上传的数据，依照格式填写
************************************************************
*/
bool ONENET_CLASS::OneNet_SendData(void)
{
  
    char send_buf[256];
    
    if(!ONENET_CLASS::netWork || NET_DEVICE.NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)    //如果网络未连接 或 不为数据收发模式
        return 1;
    
    NET_DEVICE.NET_DEVICE_Set_DataMode(DEVICE_CMD_MODE);                   //设置为命令收发模式
    memset(send_buf, 0, sizeof(send_buf));

    sprintf(send_buf, ",;led1Sta,%d;led2Sta,%d;beep,%d;light,%d;temperature,%d;humidity,%d;errType,%d;",

                      LED.led1Sta,LED.led2Sta,BEEP.beepSta,LIGHT.light,
                      SHT20.tempreture,SHT20.humidity,
                      FAULT.faultTypeReport);
          
    EDPKIT.PacketSavedataSimpleString(NULL, send_buf);              //封包
    
    NET_DEVICE.NET_DEVICE_SendData(EDPKIT.GetData(), EDPKIT.GetWritepos());   //上传平台

    EDPKIT.ClearParameter();                 //删包
    
    FAULT.faultTypeReport = FAULT_NONE;                           //发送之后清除标记
    
    NET_DEVICE.NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);
    
    return 0;
  
}

/*
************************************************************
*   函数名称： OneNet_HeartBeat
*
*   函数功能： 心跳检测
*
*   入口参数： 无
*
*   返回参数： 无
*
*   说明：   
************************************************************
*/
bool ONENET_CLASS::OneNet_HeartBeat(void)
{

    unsigned char heartBeat[2] = {PINGREQ, 0}, sCount = 5;
    unsigned char errType = 0;
    unsigned char *dataPtr;
    
    if(!ONENET_CLASS::netWork || NET_DEVICE.NET_DEVICE_Get_DataMode() != DEVICE_DATA_MODE)  //如果网络为连接 或 不为数据收发模式
        return 0;
    
    NET_DEVICE.NET_DEVICE_Set_DataMode(DEVICE_HEART_MODE);                 //设置为心跳收发模式
    
    while(--sCount)                               //循环检测计数
    {
        NET_DEVICE.NET_DEVICE_SendData(heartBeat, sizeof(heartBeat));            //向平台上传心跳请求
    
        dataPtr = NET_DEVICE.NET_DEVICE_GetIPD(200);                   //获取数据，等到2s
        if(dataPtr != NULL)                           //如果数据指针不为空
        {
            if(dataPtr[0] == PINGRESP)                      //第一个数据为心跳响应
            {
                errType = CHECK_NO_ERR;                     //标记无错误
                break;
            }
        }
        else
        {
            if(dataPtr[0] != PINGRESP)                      //心跳未响应
            {
                errType = NET_DEVICE.NET_DEVICE_Check();                 //网络设备状态检查
            }
        }
          
        delay(100);                            //延时等待
        NET_DEVICE.NET_DEVICE_ClrData();                           //情空缓存
    }
    
    if(sCount == 0)                               //超出
    {
        //errType = NET_DEVICE.NET_DEVICE_Check();                     //网络设备状态检查
        errType = CHECK_CLOSED;
    }
    
    if(errType == CHECK_CONNECTED || errType == CHECK_CLOSED || errType == CHECK_GOT_IP)
        FAULT.faultTypeReport = FAULT.faultType = FAULT_EDP;                //标记为协议错误
    else if(errType == CHECK_NO_DEVICE)
        FAULT.faultTypeReport = FAULT.faultType = FAULT_NODEVICE;             //标记为设备错误
    else
        FAULT.faultTypeReport = FAULT.faultType = FAULT_NONE;               //无错误
    
    NET_DEVICE.NET_DEVICE_ClrData();                           //情况缓存
    
    NET_DEVICE.NET_DEVICE_Set_DataMode(DEVICE_DATA_MODE);                  //设置为数据收发模式

    return 0;

}

/*
************************************************************
*   函数名称： OneNet_Replace
*
*   函数功能： 替换平台下发命令中的结束符
*
*   入口参数： res：平台下发的命令
*
*   返回参数： 无
*
*   说明：     平台开关、旋钮下发的命令会有固定4个结束符，替换为其他不常用字符，以方便string库解析
************************************************************
*/
void OneNet_Replace(unsigned char *res)
{
  
    unsigned char count = 0;
  
    while(count <= 4)
    {
        if(*res == '\0')    //找到结束符
        {
            *res = '~';     //替换
            count++;
        }
        
        res++;
    }

}

/*
************************************************************
*   函数名称： OneNet_App
*
*   函数功能： 平台下发命令解析、处理
*
*   入口参数： cmd：平台下发的命令
*
*   返回参数： 无
*
*   说明：     提取出命令，响应处理
************************************************************
*/
bool ONENET_CLASS::OneNet_App(unsigned char *cmd)
{

    char *dataPtr;
    char numBuf[10];
    int num = 0;
    bool flag = 0;
    
    OneNet_Replace(cmd);
    
    dataPtr = strstr((const char *)cmd, "}");     //搜索'}'
  
    if(dataPtr != NULL)                 //如果找到了
    {
        dataPtr++;
        
        while(*dataPtr >= '0' && *dataPtr <= '9')   //判断是否是下发的命令控制数据
        {
            numBuf[num++] = *dataPtr++;
        }
        
        num = atoi((const char *)numBuf);       //转为数值形式
        
        if(strstr((char *)cmd, "led1"))       //搜索"redled"
        {
            if(num == 1)                //控制数据如果为1，代表开
            {
                LED.Led1_Set(LED_ON);
            }
            else if(num == 0)             //控制数据如果为0，代表关
            {
                LED.Led1_Set(LED_OFF);
            }
            
            flag = 1;          //标记数据反馈
        }
                                //下同
        else if(strstr((char *)cmd, "led2"))
        {
            if(num == 1)
            {
                LED.Led2_Set(LED_ON);
            }
            else if(num == 0)
            {
                LED.Led2_Set(LED_OFF);
            }
            
            flag = 1;
        }
        else if(strstr((char *)cmd, "beep"))
        {
            if(num == 1)
            {
                BEEP.Beep_Set(BEEP_ON);
            }
            else if(num == 0)
            {
                BEEP.Beep_Set(BEEP_OFF);
            }
            
            flag = 1;
        }
    }
    
    NET_DEVICE.NET_DEVICE_ClrData();               //清空缓存

    return flag;
}
