#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "img.h"
#include <omp.h>
#define XWIDTH 2300
#define YWIDTH 1848
#define INPUTPATH "./images/"
#define OUTPUTPATH "./outimages/"

// Assume that memory for storing temporary results is provided by runtime system
int tmpimgh[XWIDTH][YWIDTH];
int tmpimgv[XWIDTH][YWIDTH];

static const char *const task_greyscale = "greyscale";
static const char *const task_checkcontrast = "checkcontrast";
static const char *const task_sharpencontrast = "sharpencontrast";
static const char *const task_copyimage = "copyimage";
static const char *const task_sobelh = "sobelh";
static const char *const task_sobelv = "sobelv";
static const char *const task_combineimgs = "combineimgs";
static const char *const task_writeimage = "writeimage";


int maxim(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int minim(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}

int checkcontrast(MyIMG* img)
{
    int pix, mean, squaremean, var;

    printf("Checking contrast...\n");

    mean = 0;
    squaremean = 0;
    pix = (int)getpixel(img, 4, 4);
    mean += pix;
    squaremean += pix * pix;
    pix = (int)getpixel(img, 20, 4);
    mean += pix;
    squaremean += pix * pix;
    pix = (int)getpixel(img, 4, 20);
    mean += pix;
    squaremean += pix * pix;
    pix = (int)getpixel(img, 20, 20);
    mean += pix;
    squaremean += pix * pix;
    mean = mean / 4;
    squaremean = squaremean / 4;
    var = squaremean - mean * mean;
    if (var > mean * mean / 36)
        return 1;
    return 0;
}

void sharpencontrast(MyIMG* img)
{
    int pixmax, pixmin, pixdiff;
    int x, y, xwidth, ywidth;
    int newval;

    printf("Enhancing contrast...\n");
        
    #define STEP 1 // every STEP pixel is used to find max pixel value
    pixmax = 0;
    pixmin = 255;
    xwidth = getwidth(img, XDIM);
    ywidth = getwidth(img, YDIM);
    for (y = 0; y < ywidth; y++)
        for (x = y % STEP; x < xwidth; x += STEP)
        {
            pixmax = maxim(pixmax, (int)getpixel(img, x, y));
            pixmin = minim(pixmin, (int)getpixel(img, x, y));
        }
    pixdiff = pixmax - pixmin;
    if(pixdiff == 0) pixdiff = 1;
    for (y = 0; y < ywidth; y++ )
        for (x = 0; x < xwidth; x++)
        {
            newval = (((int)getpixel(img, x, y) - pixmin) * 255) / pixdiff;
            setpixel(img, x, y, (unsigned char)newval);
        }
}

void sobelv(MyIMG* img)
{
    int x, y, xwidth, ywidth;
    int val, maxval;
    double scalingfactor;

    printf("Applying vertical sobel filter...\n");
    xwidth = getwidth(img, XDIM);
    ywidth = getwidth(img, YDIM);
    maxval = 0;
    for (y = 1; y < ywidth - 1; y++)
        for (x = 1; x < xwidth - 1; x++)
        {
            val = (int)getpixel(img, x - 1, y - 1) + 2 * (int)getpixel(img, x - 1, y) + (int)getpixel(img, x - 1, y + 1);
            val -= (int)getpixel(img, x + 1, y - 1) + 2 * (int)getpixel(img, x + 1, y) + (int)getpixel(img, x + 1, y + 1);
            if (val < 0)
                val = -val;
            if (val > maxval)
                maxval = val;
            tmpimgv[x][y] = val;
        }
    if(maxval == 0) maxval = 1;
    scalingfactor = (double)255 / maxval;
    //printf("global maximum value sobelv: %d, scaling factor: %f\n", maxval, scalingfactor);

    for (y = 1; y < ywidth - 1; y++)
        for (x = 1; x < xwidth - 1; x++)
            setpixel(img, x, y, tmpimgv[x][y]*scalingfactor);
}

void sobelh(MyIMG* img)
{
    int x, y, xwidth, ywidth;
    int val, maxval;
    double scalingfactor;

    printf("Applying horizontal sobel filter...\n");
    xwidth = getwidth(img, XDIM);
    ywidth = getwidth(img, YDIM);
    maxval = 0;
    for (y = 1; y < ywidth - 1; y++)
        for (x = 1; x < xwidth - 1; x++)
        {
            val = (int)getpixel(img, x - 1, y + 1) + 2 * (int)getpixel(img, x, y + 1) + (int)getpixel(img, x + 1, y + 1);
            val -= (int)getpixel(img, x - 1, y - 1) + 2 * (int)getpixel(img, x, y - 1) + (int)getpixel(img, x + 1, y - 1);
            if (val < 0)
                val = -val;
            if (val > maxval)
                maxval = val;
            tmpimgh[x][y] = val;
        }
    if(maxval == 0) maxval = 1;
    scalingfactor = (double)255 / maxval;
    //printf("global maximum value sobelh: %d, scaling factor: %f\n", maxval, scalingfactor);

    for (y = 1; y < ywidth - 1; y++)
        for (x = 1; x < xwidth - 1; x++)
            setpixel(img, x, y, tmpimgh[x][y]*scalingfactor);
}

void greyscale(MyIMG* imgrgb,MyIMG* img)
{
    int x, y, xwidth, ywidth;
    int red, green, blue, grey;

    printf("Converting to greyscale...\n");

    xwidth = getwidth(imgrgb, XDIM);
    ywidth = getwidth(imgrgb, YDIM);
    for (y = 0; y < ywidth; y++)
        for (x = 0; x < xwidth; x++)
        {
            red = (int)getpixelrgb(imgrgb, x, y, 0);
            green = (int)getpixelrgb(imgrgb, x, y, 1);
            blue = (int)getpixelrgb(imgrgb, x, y, 2);
            grey = (int)(0.2126 * red + 0.7152 * green + 0.0727 * blue);
            setpixel(img, x, y, (unsigned char)grey);
        }
}

void combineimgs(MyIMG* imgh, MyIMG* imgv)
{
    int x, y, xwidth, ywidth;
    int valh, valv;

    printf("Combining images...\n");
    xwidth = getwidth(imgh, XDIM);
    ywidth = getwidth(imgh, YDIM);

    for (y = 1; y < ywidth - 1; y++)
        for (x = 1; x < xwidth - 1; x++)
        {
            valh = (int)getpixel(imgh, x, y);
            valv = (int)getpixel(imgv, x, y);
            valv = valh * valh + valv * valv;
            if (valv <= 0)
                valh = 0;
            else
                valh = (int)sqrt((double)valv);
            setpixel(imgh, x, y, (unsigned char)valh);
        }
}

void loadimage(MyIMG* img, char* filename)
{
    unsigned char *dataptr;
    FILE *fp;
    char infilename[128];

    snprintf(infilename, sizeof(infilename), "%s%s.bin", INPUTPATH, filename);
    printf("Reading image %s from disk...\n", infilename);
    
    dataptr = img->ptr;
    
    fp = fopen(infilename, "r");
    fread(dataptr, sizeof(unsigned char), XWIDTH*YWIDTH*3, fp);
    fclose(fp);
}

void writeimage(MyIMG* img, char* filename)
{
    // Write image to binary file
    int xwidth;
    int ywidth;
    unsigned char *data;
    FILE *fp;
    char outfilename[128];

    snprintf(outfilename, sizeof(outfilename), "%s%s_COMBINED.bin", OUTPUTPATH, filename);
    printf("Writing image %s to disk...\n", outfilename);

    xwidth = img->xwidth;
    ywidth = img->ywidth;
    data = img->ptr;
    fp = fopen(outfilename, "w");
    fwrite(data,sizeof(unsigned char),xwidth*ywidth,fp);
    fclose(fp);
}

void runEdgedetection(char* taskname, char* filename, MyIMG* imgrgb, MyIMG* imgh) {
    MyIMG *imgv;
    loadimage(imgrgb,filename);

    if (taskname==NULL) {
        greyscale(imgrgb,imgh);
        checkcontrast(imgh);
        sharpencontrast(imgh);
        copyimage(&imgv,imgh);
        sobelh(imgh);
        sobelv(imgv);
        combineimgs(imgh,imgv);
        writeimage(imgh,filename);
    } else if (strcmp(taskname, task_greyscale) == 0) {
        greyscale(imgrgb,imgh);
    } else if (strcmp(taskname, task_checkcontrast) == 0) {
        checkcontrast(imgh);
    } else if (taskname==NULL || strcmp(taskname, task_sharpencontrast) == 0) {
        sharpencontrast(imgh);
    } else if (strcmp(taskname, task_copyimage) == 0) {
        copyimage(&imgv,imgh);
    } else if (taskname==NULL || strcmp(taskname, task_sobelh) == 0) {
        sobelh(imgh);
    } else if (taskname==NULL || strcmp(taskname, task_sobelv) == 0) {
        sobelv(imgv);
    } else if (strcmp(taskname, task_combineimgs) == 0) {
        copyimage(&imgv,imgh);
        combineimgs(imgh,imgv);
    } else if (taskname==NULL || strcmp(taskname, task_writeimage) == 0) {
        writeimage(imgh,filename);
    }
}

int main(int argc,char *argv[])
{
    char *listfilename;
    char filename[64];
    FILE *fp;
    MyIMG *imgrgb, *imgh;

    listfilename = argv[1];
    fp = fopen(listfilename, "r");
    imgrgb = createimagergb(XWIDTH, YWIDTH);
    imgh = createimage(XWIDTH, YWIDTH);
    int parallelism = (argc>2) ? atoi(argv[2]) : 1;
    char* taskname = (argc>3) ? argv[3] : NULL;


    printf("Parallelitätsgrad: %d\n", parallelism);
    printf("Task: %s\n", taskname);
    char filenames[23][64];

    int counter = 0;
    while(fgets(filename, sizeof(filename), fp) != NULL) {
        filename[strcspn(filename, "\n")] = 0;
        strcpy(filenames[counter], filename );
        counter++;
    }

    if (parallelism<=1) {
        for (int i = 0; i<counter; i++)
        {
            runEdgedetection(taskname, filenames[i], imgrgb, imgh);
        }
        return 0;
    }

    printf("dyn: %d\n", omp_get_dynamic());
    printf("num_treads: %d\n", omp_get_num_threads());
    omp_set_num_threads(parallelism); // Parallelitätsgrad über OMP gesetzt.

    #pragma omp parallel
    {
        #pragma omp for
        for (int j = 0; j < counter; j++) {
            runEdgedetection(taskname, filenames[j], imgrgb, imgh);
        }
    };

    //destroyimage(imgrgb);
    //destroyimage(imgh);
    //destroyimage(imgv);
    return 0;
}
