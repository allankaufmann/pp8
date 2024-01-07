
#include "ebloads.cpp"
#include <string.h>



static const char *const m4x4smul_SIMD = "m4x4smul_SIMD";

static const char *const v1x4smul_SIMD = "v1x4smul_SIMD";

static const char *const dmul64_SIMD = "dmul64_SIMD";

static const char *const smul32_SIMD = "smul32_SIMD";

static const char *const ssub32_SIMD = "ssub32_SIMD";

static const char *const dsub64_SIMD = "dsub64_SIMD";

static const char *const dmem64_SIMD = "dmem64_SIMD";

static const char *const smem32_SIMD = "smem32_SIMD";

static const char *const vmov_SIMD = "vmov_SIMD";

static const char *const vconvert_SIMD = "vconvert_SIMD";

static const char *const m4x4smul = "m4x4smul";

static const char *const dmul = "dmul";

static const char *const dadd = "dadd";

static const char *const imul = "imul";

static const char *const iadd = "iadd";

static const char *const icompare = "icompare";

static const char *const logic = "logic";

static const char *const branch = "branch";

static const char *const imem = "imem";

static const char *const dmem = "dmem";

static const char *const imov = "imov";

static const char *const shift = "shift";

static const char *const bitbyte = "bitbyte";

static const char *const nop = "nop";




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
        else if (strcmp(taskname, dsub64_SIMD) == 0)
        {
            run_dsub64_SIMD(1);
            printf("run_%s wurde ausgefuehrt", dsub64_SIMD);
        }
        else if (strcmp(taskname, dmem64_SIMD) == 0)
        {
            run_dmem64_SIMD(1);
            printf("run_%s wurde ausgefuehrt", dmem64_SIMD);
        }
        else if (strcmp(taskname, smem32_SIMD) == 0)
        {
            run_smem32_SIMD(1);
            printf("run_%s wurde ausgefuehrt", smem32_SIMD);
        }
        else if (strcmp(taskname, vmov_SIMD) == 0)
        {
            run_vmov_SIMD(1);
            printf("run_%s wurde ausgefuehrt", vmov_SIMD);
        }
        else if (strcmp(taskname, vconvert_SIMD) == 0)
        {
            run_vconvert_SIMD(1);
            printf("run_%s wurde ausgefuehrt", vconvert_SIMD);
        }
        else if (strcmp(taskname, m4x4smul) == 0)
        {
            run_m4x4smul(1);
            printf("run_%s wurde ausgefuehrt", m4x4smul);
        }
        else if (strcmp(taskname, dmul) == 0)
        {
            run_dmul(1);
            printf("run_%s wurde ausgefuehrt", dmul);
        }
        else if (strcmp(taskname, imul) == 0)
        {
            run_imul(1);
            printf("run_%s wurde ausgefuehrt", imul);
        }
        else if (strcmp(taskname, iadd) == 0)
        {
            run_iadd(1);
            printf("run_%s wurde ausgefuehrt", iadd);
        }
        else if (strcmp(taskname, icompare) == 0)
        {
            run_icompare(1);
            printf("run_%s wurde ausgefuehrt", icompare);
        }
        else if (strcmp(taskname, logic) == 0)
        {
            run_logic(1);
            printf("run_%s wurde ausgefuehrt", logic);
        }
        else if (strcmp(taskname, branch) == 0)
        {
            run_branch(1);
            printf("run_%s wurde ausgefuehrt", branch);
        }
        else if (strcmp(taskname, imem) == 0)
        {
            run_imem(1);
            printf("run_%s wurde ausgefuehrt", imem);
        }
        else if (strcmp(taskname, dmem) == 0)
        {
            run_dmem(1);
            printf("run_%s wurde ausgefuehrt", dmem);
        }
        else if (strcmp(taskname, imov) == 0)
        {
            run_imov(1);
            printf("run_%s wurde ausgefuehrt", imov);
        }
        else if (strcmp(taskname, shift) == 0)
        {
            run_shift(1);
            printf("run_%s wurde ausgefuehrt", shift);
        }
        else if (strcmp(taskname, bitbyte) == 0)
        {
            run_bitbyte(1);
            printf("run_%s wurde ausgefuehrt", bitbyte);
        }
        else if (strcmp(taskname, nop) == 0)
        {
            run_nop(1);
            printf("run_%s wurde ausgefuehrt", nop);
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
