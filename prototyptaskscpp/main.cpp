
#include "ebloads.cpp"
#include <string.h>

int main(int argc,char *argv[]) {

    int i;
    printf("Anzahl Argumente: %d\n",argc);
    for(i=0;i<argc;i++)
    {
        printf("%d. Argument: %s\n",i, argv[i]);
    }

    if (argc==2) {
        printf("Es wurden zwei Argumente uebergeben, das 1. Element wird ausgewertet!\n");
        if (strcmp(argv[1], "dadd") == 0)
        {
            printf("run_dadd ");
            run_dadd(1);
            printf("wurde ausgeführt\n");
        }
        else if (strcmp(argv[1], "m4x4smul_SIMD") == 0)
        {
            printf("run_m4x4smul_SIMD ");
            run_m4x4smul_SIMD(1);
            printf("wurde ausgeführt\n");
        }
        else if (strcmp(argv[1], "v1x4smul_SIMD") == 0)
        {
            printf("run_v1x4smul_SIMD");
            run_v1x4smul_SIMD(1);
            printf("wurde ausgeführt\n");
        }

        else /* default: */
        {
            printf("Unbekannter Wert, der Wert war: [%s]", argv[1]);
        }

    } else {
        printf("enter parameter e. g. dadd");
    }

    return 0;
}
