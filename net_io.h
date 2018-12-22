#ifndef _NET_IO_H_
#define _NET_IO_H_

class NET_IO_CLASS
{

    public:
        void NET_IO_Init(void);

        void NET_IO_Send(unsigned char *str, unsigned short len);
        
        bool NET_IO_WaitRecive(void);
        
        void NET_IO_ClearRecive(void);

        unsigned char buf[64];     //接收缓存

    private:
        unsigned char dataLen;      //接收数据长度
        unsigned char dataLenPre;    //上一次的长度数据，用于比较

};

#define REV_OK    0 //接收完成标志
#define REV_WAIT  1 //接收未完成标志

#endif
