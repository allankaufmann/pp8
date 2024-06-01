#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "img.h"
#include <omp.h>
#include <stdint.h>//uint64_t
#include <sys/time.h>
#include <stdbool.h>
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
static const char *const task_loadimage = "loadimage";

bool withPThread = true; // try pthread for more cores

void greyscale(MyIMG* imgrgb,MyIMG* img);
int checkcontrast(MyIMG* img);
void sharpencontrast(MyIMG* img);
void sobelh(MyIMG* img);
void sobelv(MyIMG* img);
void combineimgs(MyIMG* imgh, MyIMG* imgv);
void writeimage(MyIMG* img, char* filename);
void loadimage(imgrgb,filename);

typedef void (*FunctionPtrWithImgrgbAndImg)(MyIMG* imgrgb,MyIMG* img);
typedef void (*FunctionPtrWithImg)(MyIMG* img);
typedef void (*FunctionPtrWithImgAndChar)(MyIMG* img, char* filename);



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

long unsigned  readEnergy_UJ() {
    FILE *filePointer;
    filePointer = popen("cat /sys/class/powercap/intel-rapl/intel-rapl\\:0/energy_uj", "r");

    long unsigned energy_ui=0;

    fscanf(filePointer, "%lu", &energy_ui);
    pclose(filePointer);
    return energy_ui;
}

void smtoff() {
    system("echo off > /sys/devices/system/cpu/smt/control");
}

void smton() {
    system("echo on > /sys/devices/system/cpu/smt/control");
}

void prepareParallelism(int parallelism) {
    printf("dyn: %d\n", omp_get_dynamic());
    printf("num_treads: %d\n", omp_get_num_threads());
    smtoff();
}

unsigned long long  millisecondsSinceEpoch() {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    unsigned long long millisecondsSinceEpoch =
            (unsigned long long)(tv.tv_sec) * 1000 +
            (unsigned long long)(tv.tv_usec) / 1000;

    return millisecondsSinceEpoch;
}

bool checkDuration(unsigned long long timestamp_begin) {
    unsigned long long timestamp_end = millisecondsSinceEpoch();
    unsigned long long duration = timestamp_end - timestamp_begin;
    return duration > 5000;
}

// Struktur für die Thread-Parameter
typedef struct {
    char* taskname;
    unsigned long long timestamp_begin;
    char* paramFilename;
    MyIMG* paramImgrgb;
    MyIMG* paramImgh;
    MyIMG  *imgv;
} ThreadParams;


void *thread_function(void *arg) {
    ThreadParams *params = (ThreadParams *)arg;
    printf("Parameter 1: %s\n", params->taskname);
    printf("Parameter 2: %d\n", params->timestamp_begin);
    bool stop = false;


    if(params->taskname == NULL) {
        while(!stop) {
            checkcontrast(params->paramImgh);
            loadimage(params->paramImgrgb,params->paramFilename);
            greyscale(params->paramImgrgb,params->paramImgh);
            sharpencontrast(params->paramImgh);
            copyimage(&params->imgv,params->paramImgh);
            sobelh(params->paramImgh);
            sobelv(params->imgv);
            combineimgs(params->paramImgh,params->imgv);
            writeimage(params->paramImgh,params->paramFilename);
            stop= checkDuration(params->timestamp_begin);
        }
    } else if (strcmp(params->taskname, task_checkcontrast) == 0) {
        while(!stop) {
            checkcontrast(params->paramImgh);
            stop= checkDuration(params->timestamp_begin);
        }
    } else if (strcmp(params->taskname, task_loadimage) == 0) {
        while(!stop) {
            loadimage(params->paramImgrgb,params->paramFilename);
            stop= checkDuration(params->timestamp_begin);
        }
    } else if (strcmp(params->taskname, task_greyscale) == 0) {
        while(!stop) {
            greyscale(params->paramImgrgb,params->paramImgh);
            stop= checkDuration(params->timestamp_begin);
        }
    } else if (strcmp(params->taskname, task_sharpencontrast) == 0) {
        while(!stop) {
            sharpencontrast(params->paramImgh);
            stop= checkDuration(params->timestamp_begin);
        }
    } else if (strcmp(params->taskname, task_copyimage) == 0) {
        while(!stop) {
            copyimage(&params->imgv,params->paramImgh);
            stop= checkDuration(params->timestamp_begin);
        }
    } else if (strcmp(params->taskname, task_sobelh) == 0) {
        while(!stop) {
            sobelh(params->paramImgh);
            stop= checkDuration(params->timestamp_begin);
        }
    } else if (strcmp(params->taskname, task_sobelv) == 0) {

        while(!stop) {
            sobelv(params->imgv);
            stop= checkDuration(params->timestamp_begin);
        }
    } else if (strcmp(params->taskname, task_combineimgs) == 0) {
        while(!stop) {
            combineimgs(params->paramImgh,params->imgv);
            stop= checkDuration(params->timestamp_begin);
        }
    } else if (strcmp(params->taskname, task_writeimage) == 0) {
        while(!stop) {
            writeimage(params->paramImgh,params->paramFilename);
            stop= checkDuration(params->timestamp_begin);
        }
    }
}

ThreadParams prepareTask(ThreadParams params) {
    loadimage(params.paramImgrgb,params.paramFilename);

    MyIMG *imgv;
    if (strcmp(params.taskname, task_combineimgs) == 0 || strcmp(params.taskname, task_sobelv) == 0) {
        copyimage(&imgv,params.paramImgh);
        params.imgv=imgv;
    }
    return params;
}

void measureAppTaskWithPThread(char* taskname, int parallelism, ThreadParams params) {
    params = prepareTask(params);
    prepareParallelism(parallelism);
    unsigned long long timestamp_begin = millisecondsSinceEpoch();
    long long counter_begin = readEnergy_UJ();
    params.timestamp_begin=timestamp_begin;

    pthread_t threads[parallelism];
    for (int j = 0; j < parallelism; j++) {
        pthread_create(&threads[j], NULL, thread_function, (void *)&params);
    }
    for (int j = 0; j < parallelism; j++) {
        pthread_join(threads[j], NULL);
    }

    long long counter_end = readEnergy_UJ();
    unsigned long long timestamp_end = millisecondsSinceEpoch();

    long long counter_diff = counter_end - counter_begin;
    unsigned long long duration = timestamp_end - timestamp_begin;
    printf("dauer %llu, zähler %llu, power %llu\n", duration, counter_diff, counter_diff/duration);

    mkdir("measure", 0777);
    char* filename = (char*) malloc(sizeof(char) * 30);
    sprintf(filename, "%s%s.log", "measure/", taskname);
    FILE* log = fopen(filename, "w");
    fprintf(log, "parallelism=%d\n", parallelism);
    fprintf(log, "duration=%lld\n", duration);
    fprintf(log, "counter_diff=%lld\n", counter_diff);
    fclose(log);
    free(filename);
}

void callFunctionWithImgrgbAndImg(char* taskname, FunctionPtrWithImgrgbAndImg functionPtrWithImgrgbAndImg, int parallelism, MyIMG* imgrgb,MyIMG* img) {
    prepareParallelism(parallelism);
    unsigned long long timestamp_begin = millisecondsSinceEpoch();
    long long counter_begin = readEnergy_UJ();
    bool stop = false;

    #pragma omp parallel num_threads(parallelism) shared(timestamp_begin, stop)
        {
            #pragma omp for
                for (int j = 0; j < parallelism; j++) {
                    int counter = 1;
                    while(!stop) {
                        printf("OMP-Lauf=%d, CPU %d / Durchgang#%d, Thread %d\n", j, sched_getcpu(), counter++, omp_get_thread_num());
                        functionPtrWithImgrgbAndImg(imgrgb, img);
                        stop=checkDuration(timestamp_begin);
                    }
                }
    };
    long long counter_end = readEnergy_UJ();
    unsigned long long timestamp_end = millisecondsSinceEpoch();

    long long counter_diff = counter_end - counter_begin;
    unsigned long long duration = timestamp_end - timestamp_begin;
    printf("dauer %llu, zähler %llu, power %llu\n", duration, counter_diff, counter_diff/duration);

    mkdir("measure", 0777);
    char* filename = (char*) malloc(sizeof(char) * 30);
    sprintf(filename, "%s%s.log", "measure/", taskname);
    FILE* log = fopen(filename, "w");
    fprintf(log, "parallelism=%d\n", parallelism);
    fprintf(log, "duration=%lld\n", duration);
    fprintf(log, "counter_diff=%lld\n", counter_diff);
    fclose(log);
    free(filename);
}

void callFunctionWithImg(char* taskname, FunctionPtrWithImg functionPtrWithImg, int parallelism, MyIMG* img) {
    prepareParallelism(parallelism);
    unsigned long long timestamp_begin = millisecondsSinceEpoch();
    long long counter_begin = readEnergy_UJ();
    bool stop = false;
    #pragma omp parallel num_threads(parallelism) shared(timestamp_begin, stop)
    {
        #pragma omp for
        for (int j = 0; j < parallelism; j++) {
            int counter = 1;
            while(!stop) {
                printf("OMP-Lauf=%d, CPU %d / Durchgang#%d, Thread %d\n", j, sched_getcpu(), counter++, omp_get_thread_num());
                functionPtrWithImg(img);
                stop=checkDuration(timestamp_begin);
            }
        }

        };
    long long counter_end = readEnergy_UJ();
    unsigned long long timestamp_end = millisecondsSinceEpoch();

    long long counter_diff = counter_end - counter_begin;
    unsigned long long duration = timestamp_end - timestamp_begin;
    printf("dauer %llu, zähler %llu, power %llu\n", duration, counter_diff, counter_diff/duration);

    mkdir("measure", 0777);
    char* filename = (char*) malloc(sizeof(char) * 30);
    sprintf(filename, "%s%s.log", "measure/", taskname);
    FILE* log = fopen(filename, "w");
    fprintf(log, "parallelism=%d\n", parallelism);
    fprintf(log, "duration=%lld\n", duration);
    fprintf(log, "counter_diff=%lld\n", counter_diff);
    fclose(log);
    free(filename);
}

void callFunctionWithImgAndChar(char* taskname, FunctionPtrWithImgAndChar functionPtrWithImgAndChar, int parallelism, MyIMG* img, char* filename) {
    prepareParallelism(parallelism);
    unsigned long long timestamp_begin = millisecondsSinceEpoch();
    long long counter_begin = readEnergy_UJ();
    bool stop = false;
    #pragma omp parallel num_threads(parallelism) shared(timestamp_begin, stop)
    {
        #pragma omp for
        for (int j = 0; j < parallelism; j++) {
            int counter = 1;
            while(!stop) {
                printf("OMP-Lauf=%d, CPU %d / Durchgang#%d, Thread %d\n", j, sched_getcpu(), counter++, omp_get_thread_num());
                functionPtrWithImgAndChar(img, filename);
                stop=checkDuration(timestamp_begin);
            }
        }

    };
    long long counter_end = readEnergy_UJ();
    unsigned long long timestamp_end = millisecondsSinceEpoch();

    long long counter_diff = counter_end - counter_begin;
    unsigned long long duration = timestamp_end - timestamp_begin;
    printf("dauer %llu, zähler %llu, power %llu\n", duration, counter_diff, counter_diff/duration);

    mkdir("measure", 0777);
    char* filename3 = (char*) malloc(sizeof(char) * 99);
    sprintf(filename3, "%s%s.log", "measure/", taskname);
    FILE* log = fopen(filename3, "w");
    fprintf(log, "parallelism=%d\n", parallelism);
    fprintf(log, "duration=%lld\n", duration);
    fprintf(log, "counter_diff=%lld\n", counter_diff);
    fclose(log);
    free(filename3);
}

void runEdgedetection(char* taskname, int parallelism, char* filename, MyIMG* imgrgb, MyIMG* imgh) {
    MyIMG *imgv;

    FunctionPtrWithImgrgbAndImg gs = greyscale;
    FunctionPtrWithImg  cc = checkcontrast;
    FunctionPtrWithImg  sc = sharpencontrast;
    FunctionPtrWithImgrgbAndImg ci = copyimage;
    FunctionPtrWithImg  sh = sobelh;
    FunctionPtrWithImg sv = sobelv;
    FunctionPtrWithImgrgbAndImg combine = combineimgs;
    FunctionPtrWithImgAndChar wi = writeimage;
    FunctionPtrWithImgAndChar li = loadimage;

    ThreadParams params;
    params.taskname=taskname;
    params.paramImgh=imgh;
    params.paramFilename=filename;
    params.paramImgrgb=imgrgb;
    params.imgv=imgv;
    params.timestamp_begin=0;

    if (withPThread) {
        measureAppTaskWithPThread(taskname, parallelism, params);
        smton();
        return;
    }

    if (taskname==NULL) {
        callFunctionWithImgAndChar(task_loadimage, li, parallelism,  imgrgb,filename);
        callFunctionWithImgrgbAndImg(task_greyscale, gs, parallelism, imgrgb,imgh);
        callFunctionWithImg(task_checkcontrast, cc, parallelism, imgh);
        callFunctionWithImg(task_sharpencontrast, sc, parallelism, imgh);
        callFunctionWithImgrgbAndImg(task_copyimage, ci, parallelism, &imgv,imgh);
        callFunctionWithImg(task_sobelh, sh, parallelism, imgh);
        callFunctionWithImg(task_sobelv, sv, parallelism, imgh);
        callFunctionWithImgrgbAndImg(task_combineimgs, combine,parallelism,  imgh,imgv);
        callFunctionWithImgAndChar(task_writeimage, wi, parallelism,  imgh,filename);
    } else if (strcmp(taskname, task_greyscale) == 0) {
        loadimage(imgrgb,filename);
        callFunctionWithImgrgbAndImg(taskname, gs, parallelism, imgrgb,imgh);
    } else if (strcmp(taskname, task_checkcontrast) == 0) {
        loadimage(imgrgb,filename);
        callFunctionWithImg(taskname, cc, parallelism, imgh);
    } else if (strcmp(taskname, task_sharpencontrast) == 0) {
        loadimage(imgrgb,filename);
        callFunctionWithImg(taskname, sc, parallelism, imgh);
    } else if (strcmp(taskname, task_copyimage) == 0) {
        loadimage(imgrgb,filename);
        callFunctionWithImgrgbAndImg(taskname, ci, parallelism, &imgv,imgh);
    } else if (strcmp(taskname, task_sobelh) == 0) {
        loadimage(imgrgb,filename);
        callFunctionWithImg(taskname, sh, parallelism, imgh);
    } else if (strcmp(taskname, task_sobelv) == 0) {
        loadimage(imgrgb,filename);
        callFunctionWithImg(taskname, sv, parallelism, imgh);
    } else if (strcmp(taskname, task_combineimgs) == 0) {
        loadimage(imgrgb,filename);
        copyimage(&imgv,imgh);
        callFunctionWithImgrgbAndImg(taskname, combine, parallelism, imgh,imgv);
    } else if (strcmp(taskname, task_writeimage) == 0) {
        loadimage(imgrgb,filename);
        callFunctionWithImgAndChar(taskname, wi, parallelism, imgh,filename);
    } else if (strcmp(taskname, task_loadimage) == 0) {
        callFunctionWithImgAndChar(task_loadimage, li, parallelism,  imgrgb,filename);
    }
    smton();
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
    int imageCounter = 0;
    while(fgets(filename, sizeof(filename), fp) != NULL) {
        filename[strcspn(filename, "\n")] = 0;
        strcpy(filenames[imageCounter], filename );
        imageCounter++;
    }

    runEdgedetection(taskname, parallelism, filenames[0], imgrgb, imgh);

    //destroyimage(imgrgb);
    //destroyimage(imgh);
    //destroyimage(imgv);
    return 0;
}
