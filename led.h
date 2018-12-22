#ifndef _LED_H_
#define _LED_H_

class LED_CLASS
{
  
    public:
        void Led_Init(void);
        void Led1_Set(bool status);
        void Led2_Set(bool status);
        
    
        bool led1Sta;
        bool led2Sta;
  
};

#define LED_ON    1
#define LED_OFF   0

#define LED1_PIN    13
#define LED2_PIN    12

#endif

