
#include <stdio.h>
#include <xmmintrin.h>
#define MEM_SIZE 1000000
#define _mm_replicate_x_ps(v) _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(0, 0, 0, 0))
#define _mm_replicate_y_ps(v) _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(1, 1, 1, 1))
#define _mm_replicate_z_ps(v) _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(2, 2, 2, 2))
#define _mm_replicate_w_ps(v) _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(3, 3, 3, 3))
#define SHUFFLE_PARAM(x, y, z, w) ((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))
#define _mm_madd_ps(a, b, c) _mm_add_ps(_mm_mul_ps((a), (b)), (c))

volatile int imemBuf[MEM_SIZE];
volatile float fmemBuf[MEM_SIZE];
extern volatile int done0;
#define CNT_DIV 0.000020

extern volatile double instcnt, max_instcnt;

typedef float __m128 __attribute__ ((__vector_size__ (16), __may_alias__));

void* run_m4x4smul_SIMD(int counts) {


/*    float m1[4*4]={1.0,     2.323,      -234.6565,        234.434,
                   234.22,  -423.434,   27.5678,            456.333,
                   1.66,    987.34,     234.345,            776.424,
                   0.34,    234.423,    555.232,            0.31};

    float m2[4*4]={1.1,     2.323,      -2234.6565,        234.434,
                   234.22,  -423.434,   274.5678,            456.333,
                   1.66,    987.34,     234.345,            776.424,
                   0.34,    234.423,    09555.232,            1.131};

    float m3[4*4]; */

    float* a = (float*)fmemBuf;
    float* b = (float*)fmemBuf;
    float* c = (float*)fmemBuf;
    int idx = 0;

    instcnt += counts*CNT_DIV;
    while ((counts--) && !done0) {

        idx = (idx + 12) % (MEM_SIZE-12);
        // load matrices a and b as column-major order
        __m128 ma_col_0 = _mm_load_ps(a + idx);
        __m128 ma_col_1 = _mm_load_ps(a + idx + 4);
        __m128 ma_col_2 = _mm_load_ps(a + idx + 8);
        __m128 ma_col_3 = _mm_load_ps(a + idx + 12);

        __m128 mb_col_0 = _mm_load_ps(b + idx);
        __m128 mb_col_1 = _mm_load_ps(b + idx + 4);
        __m128 mb_col_2 = _mm_load_ps(b + idx + 8);
        __m128 mb_col_3 = _mm_load_ps(b + idx + 12);

        // get ready to store the result
        __m128 result0;
        __m128 result1;
        __m128 result2;
        __m128 result3;

        // result = first column of B x first row of A
        result0 = _mm_mul_ps(mb_col_0, _mm_replicate_x_ps(ma_col_0));
        result1 = _mm_mul_ps(mb_col_0, _mm_replicate_x_ps(ma_col_1));
        result2 = _mm_mul_ps(mb_col_0, _mm_replicate_x_ps(ma_col_2));
        result3 = _mm_mul_ps(mb_col_0, _mm_replicate_x_ps(ma_col_3));

        // result += second column of B x second row of A
        result0 = _mm_madd_ps(mb_col_1, _mm_replicate_y_ps(ma_col_0), result0);
        result1 = _mm_madd_ps(mb_col_1, _mm_replicate_y_ps(ma_col_1), result1);
        result2 = _mm_madd_ps(mb_col_1, _mm_replicate_y_ps(ma_col_2), result2);
        result3 = _mm_madd_ps(mb_col_1, _mm_replicate_y_ps(ma_col_3), result3);

        // result += third column of B x third row of A
        result0 = _mm_madd_ps(mb_col_2, _mm_replicate_z_ps(ma_col_0), result0);
        result1 = _mm_madd_ps(mb_col_2, _mm_replicate_z_ps(ma_col_1), result1);
        result2 = _mm_madd_ps(mb_col_2, _mm_replicate_z_ps(ma_col_2), result2);
        result3 = _mm_madd_ps(mb_col_2, _mm_replicate_z_ps(ma_col_3), result3);

        // result += last column of B x last row of A
        result0 = _mm_madd_ps(mb_col_3, _mm_replicate_w_ps(ma_col_0), result0);
        result1 = _mm_madd_ps(mb_col_3, _mm_replicate_w_ps(ma_col_1), result1);
        result2 = _mm_madd_ps(mb_col_3, _mm_replicate_w_ps(ma_col_2), result2);
        result3 = _mm_madd_ps(mb_col_3, _mm_replicate_w_ps(ma_col_3), result3);

        // store the result to memory
        _mm_store_ps(c + idx, result0);
        _mm_store_ps(c+idx+4, result1);
        _mm_store_ps(c+idx+8, result2);
        _mm_store_ps(c+idx+12, result3);
    }
    return NULL;
}

int main() {
    run_m4x4smul_SIMD(1);
}
