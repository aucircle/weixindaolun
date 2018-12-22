#include <Arduino.h>

#include "key.h"

void KEY_CLASS::KEY_Init(void)
{

    pinMode(KEY1_PIN, INPUT);
    pinMode(KEY2_PIN, INPUT);

}

unsigned char KEY_CLASS::KEY_Keyboard(void)
{

    if(digitalRead(KEY1_PIN) == LOW)
    {
        //if(++KEY_CLASS::keyCount >= 6000)
        delay(10);
        {
            KEY_CLASS::keyCount = 0;
            if(digitalRead(KEY1_PIN) == LOW)
            {
                while(!digitalRead(KEY1_PIN));
                return KEY1DOWN;
            }
        }
    }

    if(digitalRead(KEY2_PIN) == LOW)
    {
        //if(++KEY_CLASS::keyCount >= 6000)
        delay(10);
        {
            KEY_CLASS::keyCount = 0;
            if(digitalRead(KEY2_PIN) == LOW)
            {
                while(!digitalRead(KEY2_PIN));
                return KEY2DOWN;
            }
        }
    }

    return KEYNONE;

}
