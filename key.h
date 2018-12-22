#ifndef _KEY_H_
#define _KEY_H_

class KEY_CLASS
{

    public:
        void KEY_Init(void);
        unsigned char KEY_Keyboard(void);

    private:
        unsigned short keyCount;

};

#define KEY1_PIN  3
#define KEY2_PIN  4

#define KEYNONE     0
#define KEY1DOWN    1
#define KEY2DOWN    2

#endif
