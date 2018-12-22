#ifndef _AT24C02_H_
#define _AT24C02_H_

class AT24C02_CLASS
{
  
    public:
        bool AT24C02_WriteByte(unsigned char regAddr, unsigned char *data);
        bool AT24C02_ReadByte(unsigned char regAddr, unsigned char *val);
        bool AT24C02_WriteBytes(unsigned char regAddr, unsigned char *buf, unsigned char num);
        bool AT24C02_ReadBytes(unsigned char regAddr, unsigned char *buf, unsigned char num);
  
};

#define AT24C02_ADDRESS   80

#endif
