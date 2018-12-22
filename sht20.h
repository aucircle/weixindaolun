#ifndef _SHT20_H_
#define _SHT20_H_

class SHT20_CLASS
{

    public:
        void SHT20_GetValue(void);
        unsigned char tempreture;
        unsigned char humidity;

};

/*SHT20 设备操作相关宏定义，详见手册*/
#define SHT20_ADDRESS  64
#define SHT20_Measurement_RH_HM  229
#define SHT20_Measurement_T_HM  227
#define SHT20_Measurement_RH_NHM  245
#define SHT20_Measurement_T_NHM  243
#define SHT20_READ_REG  231
#define SHT20_WRITE_REG  230
#define SHT20_SOFT_RESET  254

#endif

