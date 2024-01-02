#include <stdio.h>

extern volatile double instcnt=0;
#define CNT_DIV 0.000020
extern volatile int done0=0;

void* run_dadd(int counts)
{
    volatile double x = 3.21, y = 0.2, z = 1.34;

    instcnt += counts*CNT_DIV;
    while ((counts--) && !done0) {
        x+=1.121;
        y+=1.122;
        z+=1.123;
        x+=1.124;
        y+=1.125;
        z+=1.126;
        x+=1.127;
        y+=1.128;
        z+=1.129;
        x+=1.120;
        y+=1.121;
        z+=1.122;
        x+=1.123;
        y+=1.124;
        z+=1.125;
        x+=1.126;
        y+=1.127;
        z+=1.128;
        x+=1.129;
        y+=1.120;
    }
    return NULL;
}


int main() {
    run_dadd(1);
}
