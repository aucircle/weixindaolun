#ifndef _BEEP_H_
#define _BEEP_H_


class BEEP_CLASS
{
  
    public:
        void Beep_Init(void);
        void Beep_Set(bool status);
        
    
        bool beepSta;  
  
};

#define BEEP_ON   1
#define BEEP_OFF  0

#define BEEP_PIN    7

#endif

