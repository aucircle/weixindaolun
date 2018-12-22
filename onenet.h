#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

class ONENET_CLASS
{

    public:
        void OneNet_DevLink(const char* devid, const char* auth_key);

        bool OneNet_SendData(void);

        void OneNet_SendData_EDPType2(void);

        bool OneNet_HeartBeat(void);
        
        bool OneNet_EDPKitCmd(unsigned char *data);
        
        bool OneNet_App(unsigned char *cmd);
        
        char devID[15] = "507328795";
        char apiKey[35] = "WKICI=UARnRWsGtpmby9awNQd=I=";
      
        unsigned char netWork : 1;    //0-局网模式(AP)    1-互联网模式(STA)
        unsigned char sendData : 1;   //数据反馈
        unsigned char errCount : 3;   //错误计数
        unsigned char reverse : 3;

};

#define CHECK_CONNECTED      0 //已连接
#define CHECK_CLOSED      1 //已断开
#define CHECK_GOT_IP      2 //已获取到IP
#define CHECK_NO_DEVICE     3 //无设备
#define CHECK_INITIAL     4 //初始化状态
#define CHECK_NO_CARD     5 //没有sim卡
#define CHECK_NO_ERR      255 //

#define DEVICE_CMD_MODE     0   //AT命令模式
#define DEVICE_DATA_MODE    1   //平台命令下发模式
#define DEVICE_HEART_MODE   2   //心跳连接模式

#endif
