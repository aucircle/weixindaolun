#ifndef _NET_DEVICE_H_
#define _NET_DEVICE_H_

class NET_DEVICE_CLASS
{

    public:
        void NET_DEVICE_IO_Init(void);
        
        bool NET_DEVICE_Init(void);

        void NET_DEVICE_Reset(void);
        
        bool NET_DEVICE_ReLink(void);
        
        bool NET_DEVICE_SendCmd(char *cmd, char *res);
        
        void NET_DEVICE_SendData(unsigned char *data, unsigned short len);
        
        unsigned char *NET_DEVICE_GetIPD(unsigned short timeOut);
        
        void NET_DEVICE_ClrData(void);
        
        unsigned char NET_DEVICE_Check(void);
        
        void NET_DEVICE_ReConfig(unsigned char step);
        
        void NET_DEVICE_Set_DataMode(unsigned char mode);
        
        unsigned char NET_DEVICE_Get_DataMode(void);

    private:
        char staName[20] = "vivo Y66L";
        char staPass[30] = "123456789";
        
        unsigned short err : 2;     //错误类型
        unsigned short initStep : 4;  //初始化步骤
        unsigned short dataType : 4;  //设定数据返回类型--16种
        unsigned short reverse : 6;   //预留

};

#endif
