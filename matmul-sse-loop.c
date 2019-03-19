#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include<emmintrin.h>
#include<string.h>

void get_walltime(double *wct);
void set_array(float * arr, float num, int num_of_elements);
float get_sum(float * arr, int num_of_elements);

int main(void) {
    int i = 0, j = 0, k = 0;
    float *fa, *fb, *fc, *fsum, *pa, *pb, *pc;
    __m128 **vfa, **vfb, *vfsum;
    double te, ts;
    if (N % 16 != 0) {
        printf("N must be multiple of 16\n");
        exit(1);
    }
    i = posix_memalign((void **)&fa, 16, N * N * sizeof(float));
    if (i != 0) {
        printf("Problem Alocating Memory\n");
        exit(1);
    }
    i = posix_memalign((void **)&fb, 16, N * N * sizeof(float));
    if (i != 0) {
        free(fa);
        printf("Problem Alocating Memory\n");
        exit(1);
    }
    i = posix_memalign((void **)&fc, 16, N * N * sizeof(float));
    if (i != 0) {
        free(fa);
        free(fb);
        printf("Problem Alocating Memory\n");
        exit(1);
    }
    i = posix_memalign((void **)&fsum, 16, 4 * sizeof(float));
    if (i != 0) {
        free(fa);
        free(fb);
        free(fc);
        printf("Problem Alocating Memory\n");
        exit(1);
    }
    vfa = (__m128**) malloc(sizeof(__m128*) * 4);
    if(!vfa){
        free(fa);
        free(fb);
        free(fc);
        free(fsum);
        printf("Problem Alocating Memory\n");
        exit(1);
    }
    vfb = (__m128**) malloc(sizeof(__m128*) * 4);
    if(!vfa){
        free(fa);
        free(fb);
        free(fc);
        free(fsum);
        free(vfa);
        printf("Problem Alocating Memory\n");
        exit(1);
    }
    // initialisation of arrays
    set_array(fa, 2.0, N * N);
    set_array(fb, 3.0, N * N);
    set_array(fc, 20.0, N * N);
    set_array(fsum, 0.0, 4);
    vfsum = (__m128 *) fsum;
    get_walltime(&ts);
    pc = fc;
    for(k = 0; k < N; k++) {
        pb = fb;
        vfb[0] = (__m128 *) pb + 0;
        vfb[1] = (__m128 *) pb + 4;
        vfb[2] = (__m128 *) pb + 8;
        vfb[3] = (__m128 *) pb + 12;
        for ( j =0; j < N; j++){
            pa = fa + k * N;
            vfa[0] = (__m128 *) pa;
            vfa[1] = (__m128 *) pa + 4;
            vfa[2] = (__m128 *) pa + 8;
            vfa[3] = (__m128 *) pa + 12;
            set_array(fsum, 0.0, 4);
            for ( i = 0; i < N; i+=16) {
                * vfsum = _mm_add_ps(* vfsum, _mm_mul_ps(*vfa[0], *vfb[0]));
                * vfsum = _mm_add_ps(* vfsum, _mm_mul_ps(*vfa[1], *vfb[1]));
                * vfsum = _mm_add_ps(* vfsum, _mm_mul_ps(*vfa[2], *vfb[2]));
                * vfsum = _mm_add_ps(* vfsum, _mm_mul_ps(*vfa[3], *vfb[3]));
                vfa[0]++, vfa[1]++, vfa[2]++, vfa[3]++,\
                vfb[0]++, vfb[1]++, vfb[2]++, vfb[3]++;
            }
            *pc = get_sum(fsum, 4);
            pc++;
        }
    }
    get_walltime(&te);
    for (i = 0; i < N*N; i++){
        if(fc[i] != 6 * N){
            printf("wrong outcome%lf\n", fc[i]);
            break;
        }
    }
    // printf("it took %lfsec %lfMflops\n", te - ts, (unsigned long) N * N * N/((te-ts)*1e6));
    printf("%d\t%lf\t%lf\n", N, te - ts, (unsigned long) N * N * N/((te-ts)*1e6));
    
    free(fa);
    free(fb);
    free(fc);
    free(fsum);
    free(vfa);
    free(vfb);
    return 0;
}

void get_walltime(double *wct) {
    struct timeval tp;
    gettimeofday(&tp,NULL);
    *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}

void set_array(float * arr, float value, int num_of_elements){
    for (int i = 0; i < num_of_elements; i++) {
        arr[i] = value;
    }
}

float get_sum(float * arr, int num_of_elements){
    float sum = 0.0;
    for (int i = 0; i < num_of_elements; i++){
        sum += arr[i];
    }
    return sum;
}
