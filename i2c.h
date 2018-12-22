#ifndef _I2C_H_
#define _I2C_H_

class I2C_CLASS
{
  
    public:
      void I2C_Init(void);
      bool I2C_WriteByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *data);
      bool I2C_ReadByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *val);
      bool I2C_WriteBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num);
      bool I2C_ReadBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num);
      
    private:
      unsigned char i2cSpeed;
  
};

#endif
