// img.h

#define XDIM 1
#define YDIM 2

typedef struct img
{
    int xwidth;
    int ywidth;
    unsigned char *ptr;
} MyIMG;

extern MyIMG *createimage(int xwidth, int ywidth);
extern MyIMG *createimagergb(int xwidth, int ywidth);
extern void destroyimage(MyIMG *source);
extern void initimage(MyIMG *image);
extern void initimage_black(MyIMG *image);

extern MyIMG *getnextimage(void);
extern void copyimage(MyIMG **target, MyIMG *source);
extern void writefinalimage(MyIMG *source);

extern unsigned char getpixel(MyIMG *source, int x, int y);
extern unsigned char getpixelrgb(MyIMG *source, int x, int y, int channel);
void setpixel(MyIMG *source, int x, int y, unsigned char value);
extern int getwidth(MyIMG *source, int which);
