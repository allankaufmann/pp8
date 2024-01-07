#include <pthread.h>
#include <stdlib.h>

typedef struct {
    int size, row, column;
    double (*MA)[8], (*MB)[8], (*MC)[8];
} matrix_type_t;

void *thread_mult (void *w) {
    matrix_type_t * work = (matrix_type_t *) w;
    int i, row = work->row, column = work->column;
    work->MC[row][column] = 0;
    for (i = 0; i < work->size; i++)
        work->MC[row][column] += work->MA[row][i] * work->MB[i][column];
    return NULL;
}

int main() {
    int row, column, size = 8, i;
    double MA[8][8], MB[8][8], MC[8][8];
    matrix_type_t *work;
    pthread_t thread[8 * 8];
    for (row = 0; row < size; row++)
        for (column = 0; column < size; column++) {
            work = (matrix_type_t *) malloc(sizeof(matrix_type_t));
    work->size = size;
    work->row = row;
    work->column = column;
    work->MA = MA;
    work->MB = MB;
    work->MC = MC;
    pthread_create(&(thread[column + row * 8]), NULL,
           thread_mult, (void *) work);
    }
    for (i = 0; i < size * size; i++)
        pthread_join(thread[i], NULL);
}