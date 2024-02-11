#include <stdio.h>
#include "edgedetection.c"

#define XWIDTH 2300
#define YWIDTH 1848
#define INPUTPATH "./images/"
#define OUTPUTPATH "./outimages/"

static const char *const task_loadimage = "loadimage";
static const char *const task_greyscale = "greyscale";
static const char *const task_checkcontrast = "checkcontrast";
static const char *const task_sharpencontrast = "sharpencontrast";
static const char *const task_copyimage = "copyimage";
static const char *const task_sobelh = "sobelh";
static const char *const task_sobelv = "sobelv";
static const char *const task_combineimgs = "combineimgs";
static const char *const task_writeimage = "writeimage";




int main(int argc,char *argv[]) {

    int i;
    printf("Anzahl Argumente: %d\n",argc);
    for(i=0;i<argc;i++)
    {
        printf("%d. Argument: %s\n",i, argv[i]);
    }

    if (argc==2) {
        printf("Es wurden zwei Argumente uebergeben, das 1. Element wird ausgewertet!\n");
        char* taskname = argv[1];

        char filename[64]="Motorcycle";
        FILE *fp;
        MyIMG *imgrgb, *imgh, *imgv;

        //listfilename = argv[1];
        //fp = fopen(listfilename, "r");
        imgrgb = createimagergb(XWIDTH, YWIDTH);
        imgh = createimage(XWIDTH, YWIDTH);

        //sharpencontrast copyimage sobelh sobelv combineimgs writeimage
        if (strcmp(taskname, task_loadimage) == 0) {
            loadimage(imgrgb,filename);
        } else if (strcmp(taskname, task_greyscale) == 0) {
            greyscale(imgrgb,imgh);
        } else if (strcmp(taskname, task_checkcontrast) == 0) {
            checkcontrast(imgh);
        } else if (strcmp(taskname, task_sharpencontrast) == 0) {
            sharpencontrast(imgh);
        } else if (strcmp(taskname, task_copyimage) == 0) {
            copyimage(&imgv,imgh);
        } else if (strcmp(taskname, task_sobelh) == 0) {
            sobelh(imgh);
        } else if (strcmp(taskname, task_sobelv) == 0) {
            copyimage(&imgv,imgh);
            sobelv(imgv);
        } else if (strcmp(taskname, task_combineimgs) == 0) {
            copyimage(&imgv,imgh);
            combineimgs(imgh,imgv);
        } else if (strcmp(taskname, task_writeimage) == 0) {
            writeimage(imgh,filename); // Ordner outimages muss vorhanden sein
        } else /* default: */
        {
            printf("Unbekannter Wert, der Wert war: [%s]", argv[1]);
        }

    } else {
        printf("enter a taskname e. g. loadimage");
    }

    return 0;
}
