// img.c

#include <stdio.h>
#include <stdlib.h>
#include "img.h"

static const int maxXWidth = 10; // Für die Sequenzerstellung sollten 10x10 Durchgänge ausreichend sein
static const int maxYWidth = 10; // Für die Sequenzerstellung sollten 10x10 Durchgänge ausreichend sein

MyIMG *createimage(int xwidth, int ywidth)
{
    MyIMG *source = NULL;

    source = (MyIMG *)malloc(sizeof(MyIMG));
    source->ptr = (unsigned char *)malloc(xwidth * ywidth * sizeof(unsigned char));
    source->xwidth = xwidth;
    source->ywidth = ywidth;
    return source;
}

void initimage(MyIMG *image)
{
    int xwidth, ywidth;
    int i;

    xwidth = image->xwidth;
    ywidth = image->ywidth;
    unsigned char *data = image->ptr;
    for(i=0;i<xwidth*ywidth;i++)
        data[i] = (unsigned char)255;
}

void initimage_black(MyIMG *image)
{
    int xwidth, ywidth;
    int i;

    xwidth = image->xwidth;
    ywidth = image->ywidth;
    unsigned char *data = image->ptr;
    for(i=0;i<xwidth*ywidth;i++)
        data[i] = (unsigned char)0;
}

MyIMG *createimagergb(int xwidth, int ywidth)
{
    MyIMG *source = NULL;

    source = (MyIMG *)malloc(sizeof(MyIMG));
    source->ptr = (unsigned char *)malloc(xwidth * ywidth * 3 * sizeof(unsigned char));
    source->xwidth = xwidth;
    source->ywidth = ywidth;
    return source;
}

void destroyimage(MyIMG *source)
{
    free(source->ptr);
    free(source);
}

MyIMG *getnextimage(void)
{
    MyIMG *source;
    unsigned char *srcptr;
    unsigned char value;

    source = createimage(300, 300);
    srcptr = source->ptr;
    value = 1;
    for (int x = source->xwidth; x--;)
        for (int y = source->ywidth; y--;)
        {
            *srcptr++ = value;
            value = 3 * value + 1;
        }
    return source;
}

void copyimage(MyIMG **target, MyIMG *source)
{
    unsigned char *srcptr, *trgptr;
    *target = createimage(source->xwidth, source->ywidth);
    srcptr = source->ptr;
    trgptr = (*target)->ptr;
    source->xwidth=maxXWidth;
    source->ywidth=maxYWidth;
    for (int x = source->xwidth; x--;)
        for (int y = source->ywidth; y--;)
            *trgptr++ = *srcptr++;
}

void writefinalimage(MyIMG *source)
{
    destroyimage(source);
}

unsigned char getpixel(MyIMG *source, int x, int y)
{
    return *(source->ptr + y * (source->xwidth) + x);
}

// Channel is in RGB order, i.e. R -> 0, G -> 1, B -> 2
unsigned char getpixelrgb(MyIMG *source, int x, int y, int channel)
{
    return *(source->ptr + 3 * y * (source->xwidth) + 3 * x + channel);
}

void setpixel(MyIMG *source, int x, int y, unsigned char value)
{
    *(source->ptr + y * (source->xwidth) + x) = value;
}

int getwidth(MyIMG *source, int which)
{
    if (which == XDIM) {
        //return source->xwidth;
        return maxXWidth; // Für die Sequenzerstellung sollten 10x10 Durchgänge ausreichend sein
    }
    if (which == YDIM) {
        return maxYWidth;
        //return source->ywidth;
    }

    return 0;
}
