
#include "ebloads.cpp"
#include <string.h>

static const char *const dadd = "dadd";

static const char *const m4x4smul_SIMD = "m4x4smul_SIMD";

static const char *const v1x4smul_SIMD = "v1x4smul_SIMD";

static const char *const dmul64_SIMD = "dmul64_SIMD";

static const char *const smul32_SIMD = "smul32_SIMD";

static const char *const ssub32_SIMD = "ssub32_SIMD";



int main(int argc, char *argv[]) {

    int i;
    printf("Anzahl Argumente: %d\n",argc);
    for(i=0;i<argc;i++)
    {
        printf("%d. Argument: %s\n",i, argv[i]);
    }

    if (argc==2) {
        printf("Es wurden zwei Argumente uebergeben, das 1. Element wird ausgewertet!\n");
        char* taskname = argv[1];
        if (strcmp(argv[1], dadd) == 0)
        {
            run_dadd(1);
            printf("run_%s wurde ausgefuehrt", dadd);
        }
        else if (strcmp(taskname, m4x4smul_SIMD) == 0)
        {
            run_m4x4smul_SIMD(1);
            printf("run_%s wurde ausgefuehrt", m4x4smul_SIMD);
        }
        else if (strcmp(taskname, v1x4smul_SIMD) == 0)
        {
            run_v1x4smul_SIMD(1);
            printf("run_%s wurde ausgefuehrt", v1x4smul_SIMD);
        }
        else if (strcmp(taskname, dmul64_SIMD) == 0)
        {
            run_dmul64_SIMD(1);
            printf("run_%s wurde ausgefuehrt", dmul64_SIMD);
        }
        else if (strcmp(taskname, smul32_SIMD) == 0)
        {
            run_smul32_SIMD(1);
            printf("run_%s wurde ausgefuehrt", smul32_SIMD);
        }
        else if (strcmp(taskname, ssub32_SIMD) == 0)
        {
            run_ssub32_SIMD(1);
            printf("run_%s wurde ausgefuehrt", ssub32_SIMD);
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
